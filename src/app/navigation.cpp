#include "psbbv/navigation.hpp"
namespace psbbv {
void moveSelection(NavigationState& state, int direction) noexcept {
    if (state.channel_open || direction == 0) return;
    if (direction < 0) state.selected = state.selected == 0 ? Channels.size() - 1 : state.selected - 1;
    else state.selected = (state.selected + 1) % Channels.size();
}
void openSelected(NavigationState& state) noexcept {
    if (state.selected >= Channels.size()) state.selected = 0;
    state.channel_open = true;
}
void goBack(NavigationState& state) noexcept { state.channel_open = false; }
const ChannelInfo& selectedChannel(const NavigationState& state) noexcept {
    return Channels[state.selected < Channels.size() ? state.selected : 0];
}
} // namespace psbbv
