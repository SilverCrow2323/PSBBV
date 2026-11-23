#pragma once

#include <cstddef>
#include <cstdint>

namespace psbbv::nodes {

constexpr std::uint16_t HostApiMajor = 1;
constexpr std::uint16_t HostApiMinor = 0;

enum class NodeCategory : std::uint8_t {
    Games,
    Applications,
    Network,
    Media,
    Downloads,
    System
};

enum class NodeAvailability : std::uint8_t {
    Planned,
    Ready,
    Unavailable,
    Disabled,
    Error
};

enum class NodePlatform : std::uint8_t {
    Vita = 1u << 0u,
    Windows = 1u << 1u,
    Linux = 1u << 2u,
    MacOS = 1u << 3u,
    Android = 1u << 4u
};

constexpr std::uint8_t platformMask(NodePlatform platform) noexcept {
    return static_cast<std::uint8_t>(platform);
}

struct NodeDefinition final {
    const char* id;
    const char* name;
    NodeCategory category;
    std::uint16_t apiMajor;
    std::uint16_t apiMinor;
    std::uint8_t platforms;
    NodeAvailability declaredAvailability;
};

enum class NodeCheck : std::uint8_t {
    Compatible,
    Planned,
    Disabled,
    Unavailable,
    Error,
    UnsupportedApiMajor,
    RequiresNewerApi,
    UnsupportedPlatform
};

std::size_t nodeCount() noexcept;
const NodeDefinition* nodeAt(std::size_t index) noexcept;
const NodeDefinition* findNode(const char* id) noexcept;

NodeCheck checkNode(
    const NodeDefinition& node,
    std::uint16_t hostApiMajor,
    std::uint16_t hostApiMinor,
    NodePlatform platform
) noexcept;

NodeAvailability effectiveAvailability(
    const NodeDefinition& node,
    std::uint16_t hostApiMajor,
    std::uint16_t hostApiMinor,
    NodePlatform platform
) noexcept;

} // namespace psbbv::nodes
