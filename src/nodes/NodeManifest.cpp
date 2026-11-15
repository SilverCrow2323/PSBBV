#include "psbbv/nodes/NodeManifest.hpp"
#include "psbbv/nodes/NodeRegistry.hpp"

#include <cctype>
#include <cstring>

namespace psbbv::nodes {
namespace {

constexpr std::size_t MaxManifestBytes = 4096;

class Reader final {
public:
    Reader(const char* data, std::size_t size) noexcept : data_(data), size_(size), pos_(0) {}

    std::size_t position() const noexcept { return pos_; }
    void whitespace() noexcept {
        while (pos_ < size_ && (data_[pos_] == ' ' || data_[pos_] == '\n' ||
               data_[pos_] == '\r' || data_[pos_] == '\t')) ++pos_;
    }
    bool take(char c) noexcept {
        whitespace();
        if (pos_ >= size_ || data_[pos_] != c) return false;
        ++pos_;
        return true;
    }
    bool string(char* out, std::size_t capacity) noexcept {
        whitespace();
        if (pos_ >= size_ || data_[pos_++] != '"' || capacity == 0) return false;
        std::size_t written = 0;
        while (pos_ < size_) {
            const unsigned char ch = static_cast<unsigned char>(data_[pos_++]);
            if (ch == '"') {
                if (written >= capacity) return false;
                out[written] = '\0';
                return true;
            }
            if (ch < 0x20u) return false;
            char value = static_cast<char>(ch);
            if (ch == '\\') {
                if (pos_ >= size_) return false;
                const char escaped = data_[pos_++];
                switch (escaped) {
                    case '"': value = '"'; break;
                    case '\\': value = '\\'; break;
                    case '/': value = '/'; break;
                    case 'b': value = '\b'; break;
                    case 'f': value = '\f'; break;
                    case 'n': value = '\n'; break;
                    case 'r': value = '\r'; break;
                    case 't': value = '\t'; break;
                    default: return false; // Unicode escapes are not needed by the ASCII node contract.
                }
            }
            if (written + 1 >= capacity) return false;
            out[written++] = value;
        }
        return false;
    }
    bool unsignedInteger(std::uint16_t& out) noexcept {
        whitespace();
        if (pos_ >= size_ || !std::isdigit(static_cast<unsigned char>(data_[pos_]))) return false;
        unsigned value = 0;
        while (pos_ < size_ && std::isdigit(static_cast<unsigned char>(data_[pos_]))) {
            value = value * 10u + static_cast<unsigned>(data_[pos_++] - '0');
            if (value > 65535u) return false;
        }
        out = static_cast<std::uint16_t>(value);
        return true;
    }
    bool skipValue(unsigned depth = 0) noexcept {
        if (depth > 8) return false;
        whitespace();
        if (pos_ >= size_) return false;
        if (data_[pos_] == '"') {
            char ignored[ManifestDescriptionCapacity];
            return string(ignored, sizeof(ignored));
        }
        if (data_[pos_] == '{') {
            ++pos_; whitespace();
            if (take('}')) return true;
            do {
                char key[64];
                if (!string(key, sizeof(key)) || !take(':') || !skipValue(depth + 1)) return false;
                whitespace();
                if (take('}')) return true;
            } while (take(','));
            return false;
        }
        if (data_[pos_] == '[') {
            ++pos_; whitespace();
            if (take(']')) return true;
            do {
                if (!skipValue(depth + 1)) return false;
                whitespace();
                if (take(']')) return true;
            } while (take(','));
            return false;
        }
        const std::size_t start = pos_;
        while (pos_ < size_ && data_[pos_] != ',' && data_[pos_] != '}' &&
               data_[pos_] != ']' && data_[pos_] != ' ' && data_[pos_] != '\n' &&
               data_[pos_] != '\r' && data_[pos_] != '\t') ++pos_;
        if (start == pos_) return false;
        const std::size_t n = pos_ - start;
        return (n == 4 && std::memcmp(data_ + start, "true", 4) == 0) ||
               (n == 5 && std::memcmp(data_ + start, "false", 5) == 0) ||
               (n == 4 && std::memcmp(data_ + start, "null", 4) == 0) ||
               (n > 0 && std::isdigit(static_cast<unsigned char>(data_[start])));
    }
    bool finished() noexcept { whitespace(); return pos_ == size_; }

private:
    const char* data_;
    std::size_t size_;
    std::size_t pos_;
};

bool copyEnumString(Reader& reader, char* value, std::size_t size) noexcept {
    return reader.string(value, size);
}

ManifestResult fail(ManifestError error, std::size_t offset) noexcept {
    return {error, offset};
}

} // namespace

ManifestResult parseNodeManifest(const char* json, std::size_t length, ParsedNodeManifest& output) noexcept {
    if (json == nullptr || length == 0) return fail(ManifestError::EmptyInput, 0);
    if (length > MaxManifestBytes) return fail(ManifestError::InputTooLarge, 0);

    ParsedNodeManifest parsed{};
    char category[24]{}, availability[24]{}, platform[24]{}, api[16]{};
    bool seen[13]{};
    Reader reader(json, length);
    if (!reader.take('{')) return fail(ManifestError::InvalidJson, reader.position());
    if (reader.take('}')) {
        return fail(ManifestError::MissingField, reader.position());
    }

    for (;;) {
        char key[40]{};
        if (!reader.string(key, sizeof(key)) || !reader.take(':'))
            return fail(ManifestError::InvalidJson, reader.position());

        int field = -1;
        if (std::strcmp(key, "manifestVersion") == 0) field = 0;
        else if (std::strcmp(key, "id") == 0) field = 1;
        else if (std::strcmp(key, "name") == 0) field = 2;
        else if (std::strcmp(key, "version") == 0) field = 3;
        else if (std::strcmp(key, "apiVersion") == 0) field = 4;
        else if (std::strcmp(key, "description") == 0) field = 5;
        else if (std::strcmp(key, "category") == 0) field = 6;
        else if (std::strcmp(key, "platforms") == 0) field = 7;
        else if (std::strcmp(key, "availability") == 0) field = 8;
        else if (std::strcmp(key, "runtime") == 0) field = 9;
        else if (std::strcmp(key, "capabilities") == 0) field = 10;
        else if (std::strcmp(key, "permissions") == 0) field = 11;
        else if (std::strcmp(key, "compatibility") == 0) field = 12;
        else return fail(ManifestError::UnknownField, reader.position());
        if (seen[field]) return fail(ManifestError::DuplicateField, reader.position());
        seen[field] = true;

        bool valid = true;
        switch (field) {
            case 0: {
                std::uint16_t version = 0;
                valid = reader.unsignedInteger(version) && version == 1;
                break;
            }
            case 1: valid = reader.string(parsed.id, sizeof(parsed.id)); break;
            case 2: valid = reader.string(parsed.name, sizeof(parsed.name)); break;
            case 3: valid = reader.string(parsed.version, sizeof(parsed.version)); break;
            case 4: valid = reader.string(api, sizeof(api)); break;
            case 5: valid = reader.string(parsed.description, sizeof(parsed.description)); break;
            case 6: valid = copyEnumString(reader, category, sizeof(category)); break;
            case 7: {
                valid = reader.take('[');
                char first[24]{};
                valid = valid && reader.string(first, sizeof(first));
                valid = valid && reader.take(']');
                std::strncpy(platform, first, sizeof(platform) - 1);
                break;
            }
            case 8: valid = copyEnumString(reader, availability, sizeof(availability)); break;
            case 9: {
                valid = reader.take('{');
                char keyName[32]{}, runtimeKind[32]{};
                valid = valid && reader.string(keyName, sizeof(keyName)) && reader.take(':') &&
                        reader.string(runtimeKind, sizeof(runtimeKind)) &&
                        std::strcmp(keyName, "kind") == 0 &&
                        (std::strcmp(runtimeKind, "builtin") == 0);
                valid = valid && reader.take('}');
                break;
            }
            case 10: valid = reader.skipValue(); break;
            default: valid = false; break;
        }
        if (!valid) return fail(ManifestError::InvalidField, reader.position());
        reader.whitespace();
        if (reader.take('}')) break;
        if (!reader.take(',')) return fail(ManifestError::InvalidJson, reader.position());
    }

    if (!reader.finished()) return fail(ManifestError::InvalidJson, reader.position());
    for (int i = 0; i <= 9; ++i) {
        if (!seen[i]) return fail(ManifestError::MissingField, reader.position());
    }

    if (std::strcmp(api, "1.0") == 0) { parsed.apiMajor = 1; parsed.apiMinor = 0; }
    else if (std::strcmp(api, "1.1") == 0) { parsed.apiMajor = 1; parsed.apiMinor = 1; }
    else return fail(ManifestError::UnsupportedValue, reader.position());

    if (std::strcmp(category, "games") == 0) parsed.category = NodeCategory::Games;
    else if (std::strcmp(category, "applications") == 0) parsed.category = NodeCategory::Applications;
    else if (std::strcmp(category, "network") == 0) parsed.category = NodeCategory::Network;
    else if (std::strcmp(category, "media") == 0) parsed.category = NodeCategory::Media;
    else if (std::strcmp(category, "downloads") == 0) parsed.category = NodeCategory::Downloads;
    else if (std::strcmp(category, "system") == 0) parsed.category = NodeCategory::System;
    else return fail(ManifestError::UnsupportedValue, reader.position());

    if (std::strcmp(platform, "psvita") != 0) return fail(ManifestError::UnsupportedValue, reader.position());
    parsed.platform = NodePlatform::Vita;

    if (std::strcmp(availability, "planned") == 0) parsed.availability = NodeAvailability::Planned;
    else if (std::strcmp(availability, "ready") == 0) parsed.availability = NodeAvailability::Ready;
    else if (std::strcmp(availability, "unavailable") == 0) parsed.availability = NodeAvailability::Unavailable;
    else if (std::strcmp(availability, "disabled") == 0) parsed.availability = NodeAvailability::Disabled;
    else return fail(ManifestError::UnsupportedValue, reader.position());

    output = parsed;
    return {ManifestError::None, reader.position()};
}

ManifestResult validateBuiltinManifest(const ParsedNodeManifest& manifest) noexcept {
    const NodeDefinition* registered = findNode(manifest.id);
    if (registered == nullptr) return fail(ManifestError::UnknownNode, 0);
    if (registered->category != manifest.category ||
        registered->apiMajor != manifest.apiMajor ||
        registered->apiMinor != manifest.apiMinor ||
        (registered->platforms & platformMask(manifest.platform)) == 0u) {
        return fail(ManifestError::IdentityMismatch, 0);
    }
    return {ManifestError::None, 0};
}

} // namespace psbbv::nodes
