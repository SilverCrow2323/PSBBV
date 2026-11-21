#include "psbbv/nodes/NodeRegistry.hpp"

#include <array>
#include <cstring>

namespace psbbv::nodes {
namespace {

constexpr std::array<NodeDefinition, 6> Registry{{
    {"factory.spdw.psbbv.games", "Games", NodeCategory::Games, 1, 0, platformMask(NodePlatform::Vita), NodeAvailability::Planned},
    {"factory.spdw.psbbv.applications", "Applications", NodeCategory::Applications, 1, 0, platformMask(NodePlatform::Vita), NodeAvailability::Planned},
    {"factory.spdw.psbbv.network", "Network", NodeCategory::Network, 1, 0, platformMask(NodePlatform::Vita), NodeAvailability::Planned},
    {"factory.spdw.psbbv.media", "Media", NodeCategory::Media, 1, 0, platformMask(NodePlatform::Vita), NodeAvailability::Planned},
    {"factory.spdw.psbbv.downloads", "Downloads", NodeCategory::Downloads, 1, 0, platformMask(NodePlatform::Vita), NodeAvailability::Planned},
    {"factory.spdw.psbbv.system", "System", NodeCategory::System, 1, 0, platformMask(NodePlatform::Vita), NodeAvailability::Planned}
}};

} // namespace

std::size_t nodeCount() noexcept {
    return Registry.size();
}

const NodeDefinition* nodeAt(std::size_t index) noexcept {
    return index < Registry.size() ? &Registry[index] : nullptr;
}

const NodeDefinition* findNode(const char* id) noexcept {
    if (id == nullptr || id[0] == '\0') {
        return nullptr;
    }
    for (const NodeDefinition& node : Registry) {
        if (std::strcmp(node.id, id) == 0) {
            return &node;
        }
    }
    return nullptr;
}

NodeCheck checkNode(
    const NodeDefinition& node,
    std::uint16_t hostApiMajor,
    std::uint16_t hostApiMinor,
    NodePlatform platform
) noexcept {
    if (node.declaredAvailability == NodeAvailability::Disabled) return NodeCheck::Disabled;
    if (node.declaredAvailability == NodeAvailability::Unavailable) return NodeCheck::Unavailable;
    if (node.declaredAvailability == NodeAvailability::Error) return NodeCheck::Error;
    if (node.apiMajor != hostApiMajor) return NodeCheck::UnsupportedApiMajor;
    if (node.apiMinor > hostApiMinor) return NodeCheck::RequiresNewerApi;
    if ((node.platforms & platformMask(platform)) == 0u) return NodeCheck::UnsupportedPlatform;
    if (node.declaredAvailability == NodeAvailability::Planned) return NodeCheck::Planned;
    return NodeCheck::Compatible;
}

NodeAvailability effectiveAvailability(
    const NodeDefinition& node,
    std::uint16_t hostApiMajor,
    std::uint16_t hostApiMinor,
    NodePlatform platform
) noexcept {
    switch (checkNode(node, hostApiMajor, hostApiMinor, platform)) {
        case NodeCheck::Compatible: return NodeAvailability::Ready;
        case NodeCheck::Planned: return NodeAvailability::Planned;
        case NodeCheck::Disabled: return NodeAvailability::Disabled;
        case NodeCheck::Error: return NodeAvailability::Error;
        case NodeCheck::Unavailable:
        case NodeCheck::UnsupportedApiMajor:
        case NodeCheck::RequiresNewerApi:
        case NodeCheck::UnsupportedPlatform:
            return NodeAvailability::Unavailable;
    }
    return NodeAvailability::Error;
}

} // namespace psbbv::nodes
