#include "psbbv/navigation/ChannelRegistry.hpp"

#include <array>

namespace psbbv::navigation {
namespace {

constexpr std::array<ChannelDefinition, 6> Registry{{
    {
        ChannelId::Games,
        "GAMES",
        "Your games, gathered in one place.",
        "A future home for local game discovery and shortcuts.",
        ChannelAvailability::Planned
    },
    {
        ChannelId::Applications,
        "APPLICATIONS",
        "Tools and homebrew, with room to grow.",
        "A future catalog for installed applications and utilities.",
        ChannelAvailability::Planned
    },
    {
        ChannelId::Network,
        "NETWORK",
        "A connected layer for digital experiences.",
        "Optional online experiences will be designed as separate modules.",
        ChannelAvailability::Planned
    },
    {
        ChannelId::Media,
        "MEDIA",
        "Your personal media, presented beautifully.",
        "A future library for locally available music, video, and images.",
        ChannelAvailability::Planned
    },
    {
        ChannelId::Downloads,
        "DOWNLOADS",
        "Keep track of content and transfers.",
        "A future workspace for transfer tasks and completed downloads.",
        ChannelAvailability::Planned
    },
    {
        ChannelId::System,
        "SYSTEM",
        "Understand and configure your Vita.",
        "A future workspace for device information and PSBBV preferences.",
        ChannelAvailability::Planned
    }
}};

} // namespace

std::size_t channelCount() noexcept {
    return Registry.size();
}

const ChannelDefinition* channelAt(std::size_t index) noexcept {
    return index < Registry.size() ? &Registry[index] : nullptr;
}

const ChannelDefinition* findChannel(ChannelId id) noexcept {
    for (const ChannelDefinition& channel : Registry) {
        if (channel.id == id) {
            return &channel;
        }
    }
    return nullptr;
}

std::size_t channelIndex(ChannelId id) noexcept {
    for (std::size_t i = 0; i < Registry.size(); ++i) {
        if (Registry[i].id == id) {
            return i;
        }
    }
    return Registry.size();
}

} // namespace psbbv::navigation
