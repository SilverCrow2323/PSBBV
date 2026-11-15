#pragma once

#include "psbbv/nodes/NodeRegistry.hpp"

#include <cstddef>
#include <cstdint>

namespace psbbv::nodes {

constexpr std::size_t ManifestIdCapacity = 129;
constexpr std::size_t ManifestNameCapacity = 65;
constexpr std::size_t ManifestVersionCapacity = 32;
constexpr std::size_t ManifestDescriptionCapacity = 281;

struct ParsedNodeManifest final {
    char id[ManifestIdCapacity]{};
    char name[ManifestNameCapacity]{};
    char version[ManifestVersionCapacity]{};
    char description[ManifestDescriptionCapacity]{};
    std::uint16_t apiMajor = 0;
    std::uint16_t apiMinor = 0;
    NodeCategory category;
    NodePlatform platform;
    NodeAvailability availability;
};

enum class ManifestError : std::uint8_t {
    None,
    EmptyInput,
    InputTooLarge,
    InvalidJson,
    DuplicateField,
    MissingField,
    UnknownField,
    InvalidField,
    UnsupportedValue,
    UnknownNode,
    IdentityMismatch
};

struct ManifestResult final {
    ManifestError error;
    std::size_t offset;
    constexpr bool ok() const noexcept { return error == ManifestError::None; }
};

// Parses the bounded, schema-validated metadata subset used by the Vita built-in host.
// This does not load code, grant permissions, or dynamically register third-party nodes.
ManifestResult parseNodeManifest(
    const char* json,
    std::size_t length,
    ParsedNodeManifest& output
) noexcept;

// Only manifests matching an already compiled-in node may pass this gate.
ManifestResult validateBuiltinManifest(
    const ParsedNodeManifest& manifest
) noexcept;

} // namespace psbbv::nodes
