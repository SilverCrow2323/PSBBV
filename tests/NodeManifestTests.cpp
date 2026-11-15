#include "psbbv/nodes/NodeManifest.hpp"
#include "psbbv/nodes/NodeRegistry.hpp"

#include <cstdlib>
#include <cstring>
#include <iostream>

namespace {
void require(bool condition, const char* message) {
    if (!condition) { std::cerr << "FAIL: " << message << '\n'; std::exit(EXIT_FAILURE); }
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
    require(result.ok(), "valid built-in manifest parses");
    require(std::strcmp(manifest.id, "factory.spdw.psbbv.media") == 0, "stable ID parsed");
    require(manifest.category == NodeCategory::Media, "category parsed");
    require(manifest.availability == NodeAvailability::Planned, "manifest cannot promote planned node");
    require(validateBuiltinManifest(manifest).ok(), "manifest resolves to compiled-in node");

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
