#include "psbbv/navigation/ChannelRegistry.hpp"

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
    using namespace psbbv::navigation;

    constexpr std::array<ChannelId, 6> expected{{
        ChannelId::Games,
        ChannelId::Applications,
        ChannelId::Network,
        ChannelId::Media,
        ChannelId::Downloads,
        ChannelId::System
    }};

    require(channelCount() == expected.size(), "registry exposes six destination channels");

    for (std::size_t i = 0; i < expected.size(); ++i) {
        const ChannelDefinition* channel = channelAt(i);
        require(channel != nullptr, "every registered index resolves");
        require(channel->id == expected[i], "channel IDs remain stable and ordered");
        require(channel->title != nullptr && channel->title[0] != '\0', "channel title is present");
        require(channel->tagline != nullptr && channel->tagline[0] != '\0', "channel tagline is present");
        require(channel->description != nullptr && channel->description[0] != '\0', "channel description is present");
        require(channel->availability == ChannelAvailability::Planned, "unimplemented channels stay explicitly planned");
        require(findChannel(channel->id) == channel, "ID lookup returns the same definition");
        require(channelIndex(channel->id) == i, "ID-to-index lookup is consistent");

        for (std::size_t j = 0; j < i; ++j) {
            require(channelAt(j)->id != channel->id, "channel IDs are unique");
        }
    }

    require(channelAt(channelCount()) == nullptr, "out-of-range access is safe");
    require(findChannel(static_cast<ChannelId>(0)) == nullptr, "unknown ID does not resolve");
    require(channelIndex(static_cast<ChannelId>(0)) == channelCount(), "unknown ID returns the documented sentinel");

    std::cout << "PSBBV channel registry: all checks passed.\n";
    return EXIT_SUCCESS;
}
