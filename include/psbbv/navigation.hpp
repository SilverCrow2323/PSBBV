#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

namespace psbbv {
enum class Channel : std::uint8_t { Games, Applications, Media, Network, Downloads, System };
struct ChannelInfo { Channel id; const char* title; const char* subtitle; };
inline constexpr std::array<ChannelInfo, 6> Channels{{
    {Channel::Games, "Games", "Your installed library"},
    {Channel::Applications, "Applications", "Homebrew and utilities"},
    {Channel::Media, "Media", "Video, music and media sources"},
    {Channel::Network, "Network", "Connectivity and transfer services"},
    {Channel::Downloads, "Downloads", "Queues and verified transfers"},
    {Channel::System, "System", "Preferences and diagnostics"},
}};
struct NavigationState { std::size_t selected = 0; bool channel_open = false; };
void moveSelection(NavigationState& state, int direction) noexcept;
void openSelected(NavigationState& state) noexcept;
void goBack(NavigationState& state) noexcept;
const ChannelInfo& selectedChannel(const NavigationState& state) noexcept;
} // namespace psbbv
