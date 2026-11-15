#include "psbbv/nodes/NodeRegistry.hpp"

#include <array>
#include <cstdlib>
#include <iostream>

namespace {

void require(bool condition, const char* message) {
    if (!condition) {
        std::cerr << "FAIL: " << message << '\n';
        std::exit(EXIT_FAILURE);
    }
}

} // namespace

int main() {
    using namespace psbbv::nodes;

    constexpr std::array<const char*, 6> ids{{
        "factory.spdw.psbbv.games",
        "factory.spdw.psbbv.applications",
        "factory.spdw.psbbv.network",
        "factory.spdw.psbbv.media",
        "factory.spdw.psbbv.downloads",
        "factory.spdw.psbbv.system"
    }};

    require(nodeCount() == ids.size(), "all six built-in nodes are registered");
    for (std::size_t i = 0; i < ids.size(); ++i) {
        const NodeDefinition* node = nodeAt(i);
        require(node != nullptr, "registered node index resolves");
        require(node->id != nullptr && node->id[0] != '\0', "node has a stable ID");
        require(node->name != nullptr && node->name[0] != '\0', "node has a display name");
        require(node->id == findNode(ids[i])->id, "registry order matches the stable node IDs");
        require(findNode(ids[i]) == node, "ID lookup returns the registered node");
        require(node->apiMajor == HostApiMajor, "built-in nodes target the host API major");
        require(node->declaredAvailability == NodeAvailability::Planned,
                "unimplemented nodes remain planned");
        require(checkNode(*node, HostApiMajor, HostApiMinor, NodePlatform::Vita) == NodeCheck::Planned,
                "planned Vita node is not reported as ready");
        require(effectiveAvailability(*node, HostApiMajor, HostApiMinor, NodePlatform::Vita)
                    == NodeAvailability::Planned,
                "effective availability preserves planned state");
        for (std::size_t j = 0; j < i; ++j) {
            require(nodeAt(j)->id != node->id, "built-in node IDs are unique");
        }
    }

    require(nodeAt(nodeCount()) == nullptr, "out-of-range access is safe");
    require(findNode(nullptr) == nullptr, "null node ID is rejected");
    require(findNode("") == nullptr, "empty node ID is rejected");
    require(findNode("factory.spdw.psbbv.unknown") == nullptr, "unknown node ID is rejected");

    NodeDefinition probe = *nodeAt(0);
    probe.declaredAvailability = NodeAvailability::Ready;
    require(checkNode(probe, HostApiMajor, HostApiMinor, NodePlatform::Vita) == NodeCheck::Compatible,
            "ready node with matching API and platform is compatible");
    require(effectiveAvailability(probe, HostApiMajor, HostApiMinor, NodePlatform::Vita)
                == NodeAvailability::Ready,
            "compatible ready node resolves to ready");

    require(checkNode(probe, 2, 0, NodePlatform::Vita) == NodeCheck::UnsupportedApiMajor,
            "different API major is rejected");
    probe.apiMinor = static_cast<std::uint16_t>(HostApiMinor + 1);
    require(checkNode(probe, HostApiMajor, HostApiMinor, NodePlatform::Vita)
                == NodeCheck::RequiresNewerApi,
            "node requiring a newer minor API is rejected");
    probe.apiMinor = HostApiMinor;
    require(checkNode(probe, HostApiMajor, HostApiMinor, NodePlatform::Windows)
                == NodeCheck::UnsupportedPlatform,
            "unsupported platform is rejected");

    probe.declaredAvailability = NodeAvailability::Disabled;
    require(checkNode(probe, HostApiMajor, HostApiMinor, NodePlatform::Vita) == NodeCheck::Disabled,
            "disabled state takes precedence");
    probe.declaredAvailability = NodeAvailability::Unavailable;
    require(effectiveAvailability(probe, HostApiMajor, HostApiMinor, NodePlatform::Vita)
                == NodeAvailability::Unavailable,
            "unavailable state is preserved");
    probe.declaredAvailability = NodeAvailability::Error;
    require(effectiveAvailability(probe, HostApiMajor, HostApiMinor, NodePlatform::Vita)
                == NodeAvailability::Error,
            "error state is preserved");

    std::cout << "PSBBV node registry: all checks passed.\n";
    return EXIT_SUCCESS;
}
