#include "psbbv/navigation.hpp"
#include <cassert>
#include <cstring>
#include <iostream>
int main() {
    using namespace psbbv;
    NavigationState state{};
    assert(state.selected == 0 && !state.channel_open);
    assert(std::strcmp(selectedChannel(state).title, "Games") == 0);
    moveSelection(state, -1);
    assert(state.selected == Channels.size() - 1);
    assert(std::strcmp(selectedChannel(state).title, "System") == 0);
    moveSelection(state, 1);
    assert(state.selected == 0);
    moveSelection(state, 1);
    assert(state.selected == 1);
    openSelected(state);
    assert(state.channel_open);
    const auto selected = state.selected;
    moveSelection(state, 1);
    assert(state.selected == selected);
    goBack(state);
    assert(!state.channel_open);
    moveSelection(state, 1);
    assert(state.selected == 2);
    openSelected(state);
    goBack(state);
    assert(state.selected == 2);
    NavigationState invalid{};
    invalid.selected = 999;
    assert(std::strcmp(selectedChannel(invalid).title, "Games") == 0);
    openSelected(invalid);
    assert(invalid.channel_open);
    std::cout << "PSBBV navigation model: all checks passed.\n";
    return 0;
}
