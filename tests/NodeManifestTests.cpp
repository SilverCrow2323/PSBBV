#include "psbbv/nodes/NodeManifest.hpp"
#include "psbbv/nodes/NodeRegistry.hpp"

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

namespace {
void require(bool condition, const char* message) {
    if (!condition) { std::cerr << "FAIL: " << message << '\n'; std::exit(EXIT_FAILURE); }
}
std::string replaceOnce(std::string source, const std::string& from, const std::string& to) {
    const std::size_t position = source.find(from);
    if (position != std::string::npos) source.replace(position, from.size(), to);
    return source;
}
constexpr const char* Valid = R"({
  "manifestVersion": 1,
  "id": "factory.spdw.psbbv.media",
  "name": "PSBBV Media",
  "version": "0.1.0",
  "apiVersion": "1.0",
  "description": "Media node",
  "category": "media",
  "platforms": ["psvita"],
  "capabilities": ["media.catalog.read"],
  "permissions": ["storage.private"],
  "availability": "planned",
  "runtime": {"kind": "builtin"}
})";
}
int main() {
    using namespace psbbv::nodes;
    ParsedNodeManifest manifest{};
    ManifestResult result = parseNodeManifest(Valid, std::strlen(Valid), manifest);
    if (!result.ok()) { std::cerr << "parse error=" << static_cast<int>(result.error) << " offset=" << result.offset << "\n"; }
    require(result.ok(), "valid built-in manifest parses");
    require(std::strcmp(manifest.id, "factory.spdw.psbbv.media") == 0, "stable ID parsed");
    require(manifest.category == NodeCategory::Media, "category parsed");
    require(manifest.availability == NodeAvailability::Planned, "manifest cannot promote planned node");
    require(manifest.capabilityCount == 1 &&
            std::strcmp(manifest.capabilities[0], "media.catalog.read") == 0,
            "capability declaration is retained in bounded parsed data");
    require(manifest.permissionCount == 1 &&
            std::strcmp(manifest.permissions[0], "storage.private") == 0,
            "permission declaration is retained in bounded parsed data");
    require(validateBuiltinManifest(manifest).ok(), "manifest resolves to compiled-in node");


    const std::string missingCapabilities = replaceOnce(
        Valid, "  \"capabilities\": [\"media.catalog.read\"],\n", "");
    result = parseNodeManifest(missingCapabilities.c_str(), missingCapabilities.size(), manifest);
    require(result.error == ManifestError::MissingField, "missing capabilities field rejected");

    const std::string missingPermissions = replaceOnce(
        Valid, "  \"permissions\": [\"storage.private\"],\n", "");
    result = parseNodeManifest(missingPermissions.c_str(), missingPermissions.size(), manifest);
    require(result.error == ManifestError::MissingField, "missing permissions field rejected");

    const std::string wrongCapabilitiesType = replaceOnce(
        Valid, "[\"media.catalog.read\"]", "\"media.catalog.read\"");
    result = parseNodeManifest(wrongCapabilitiesType.c_str(), wrongCapabilitiesType.size(), manifest);
    require(result.error == ManifestError::InvalidField, "capabilities must be an array of strings");

    const std::string malformedCapability = replaceOnce(
        Valid, "media.catalog.read", "Media.catalog.read");
    result = parseNodeManifest(malformedCapability.c_str(), malformedCapability.size(), manifest);
    require(result.error == ManifestError::InvalidField, "malformed capability identifiers rejected");

    const std::string duplicateCapability = replaceOnce(
        Valid, "[\"media.catalog.read\"]",
        "[\"media.catalog.read\", \"media.catalog.read\"]");
    result = parseNodeManifest(duplicateCapability.c_str(), duplicateCapability.size(), manifest);
    require(result.error == ManifestError::InvalidField, "duplicate capabilities rejected");

    const std::string unknownPermission = replaceOnce(
        Valid, "storage.private", "filesystem.root");
    result = parseNodeManifest(unknownPermission.c_str(), unknownPermission.size(), manifest);
    require(result.error == ManifestError::InvalidField, "unknown permissions rejected");

    const std::string duplicatePermission = replaceOnce(
        Valid, "[\"storage.private\"]",
        "[\"storage.private\", \"storage.private\"]");
    result = parseNodeManifest(duplicatePermission.c_str(), duplicatePermission.size(), manifest);
    require(result.error == ManifestError::InvalidField, "duplicate permissions rejected");

    const char* duplicate = R"({"manifestVersion":1,"manifestVersion":1})";
    result = parseNodeManifest(duplicate, std::strlen(duplicate), manifest);
    require(result.error == ManifestError::DuplicateField, "duplicate top-level keys rejected");

    const char* unknown = R"({"manifestVersion":1,"extra":true})";
    result = parseNodeManifest(unknown, std::strlen(unknown), manifest);
    require(result.error == ManifestError::UnknownField, "unknown fields rejected");

    const char* unsupported = R"({
      "manifestVersion":1,"id":"factory.spdw.psbbv.media","name":"Media",
      "version":"0.1.0","apiVersion":"9.0","description":"x","category":"media",
      "platforms":["psvita"],"capabilities":[],"permissions":[],
      "availability":"planned","runtime":{"kind":"builtin"}
    })";
    result = parseNodeManifest(unsupported, std::strlen(unsupported), manifest);
    require(result.error == ManifestError::UnsupportedValue, "unsupported API version rejected");

    result = parseNodeManifest("{}", 2, manifest);
    require(result.error == ManifestError::MissingField, "missing required fields rejected");
    result = parseNodeManifest(Valid, 4097, manifest);
    require(result.error == ManifestError::InputTooLarge, "oversized manifest rejected");

    result = parseNodeManifest(Valid, std::strlen(Valid), manifest);
    manifest.id[0] = 'x';
    require(validateBuiltinManifest(manifest).error == ManifestError::UnknownNode,
            "unknown identity cannot register as built-in");

    std::cout << "PSBBV node manifest: all checks passed.\n";
    return EXIT_SUCCESS;
}
