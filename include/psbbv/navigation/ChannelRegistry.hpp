#pragma once

#include <cstddef>
#include <cstdint>

namespace psbbv::navigation {

enum class ChannelId : std::uint8_t {
    Games = 1,
    Applications = 2,
    Network = 3,
    Media = 4,
    Downloads = 5,
    System = 6
};

enum class ChannelAvailability : std::uint8_t {
    Implemented,
    Planned,
    Unavailable,
    Disabled
};

struct ChannelDefinition final {
    ChannelId id;
    const char* title;
    const char* tagline;
    const char* description;
    ChannelAvailability availability;
};

// Home is the root view; the registry contains the six destination channels.
std::size_t channelCount() noexcept;
const ChannelDefinition* channelAt(std::size_t index) noexcept;
const ChannelDefinition* findChannel(ChannelId id) noexcept;

// Returns channelCount() when the ID is not registered.
std::size_t channelIndex(ChannelId id) noexcept;

} // namespace psbbv::navigation
