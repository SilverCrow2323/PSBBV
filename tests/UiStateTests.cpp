#include "psbbv/navigation/UiState.hpp"
#include "psbbv/navigation/MenuHitTest.hpp"
#include "psbbv/navigation/ChannelRegistry.hpp"

#include <cstdlib>
#include <iostream>

namespace {
void require(bool ok, const char* message) {
    if (!ok) { std::cerr << "FAIL: " << message << '\n'; std::exit(EXIT_FAILURE); }
}
}

int main() {
    using namespace psbbv::navigation;

    require(menuItemAtTouch(320, 238) == 0, "menu first row accepts its top-left boundary");
    require(menuItemAtTouch(639, 277) == 0, "menu first row accepts its last visible pixel");
    require(menuItemAtTouch(320, 278) == NoMenuItem, "first menu gap is not clickable");
    require(menuItemAtTouch(639, 285) == NoMenuItem, "first menu gap never selects About");
    require(menuItemAtTouch(320, 286) == 1, "menu second row starts at its visible top");
    require(menuItemAtTouch(639, 325) == 1, "menu second row accepts its last visible pixel");
    require(menuItemAtTouch(320, 326) == NoMenuItem, "second menu gap is not clickable");
    require(menuItemAtTouch(320, 334) == 2, "menu third row starts at its visible top");
    require(menuItemAtTouch(639, 373) == 2, "menu third row accepts its last visible pixel");
    require(menuItemAtTouch(320, 374) == NoMenuItem, "area below the last row is not clickable");
    require(menuItemAtTouch(319, 238) == NoMenuItem, "menu hitbox rejects the left outside edge");
    require(menuItemAtTouch(640, 238) == NoMenuItem, "menu hitbox rejects the right outside edge");
    require(menuPanelContainsTouch(320, 278), "menu row gaps remain inside the panel");
    require(menuPanelContainsTouch(500, 390), "unused panel space remains inside the panel");
    require(!menuPanelContainsTouch(299, 278), "touch outside the panel can dismiss it");
    require(!menuPanelContainsTouch(660, 200), "panel right edge is exclusive");

    require(channelCount() == 6, "six planned channel identities remain registered");
    for (std::size_t i = 0; i < channelCount(); ++i) {
        const ChannelDefinition* channel = channelAt(i);
        require(channel != nullptr, "every planned channel identity resolves");
        require(channel->availability == ChannelAvailability::Planned,
                "planned channels are not advertised as implemented");
    }

    UiState state;
    state.update(button::Up | button::Down);
    require(state.selectedChannel() == 0, "opposing directions do not focus planned channels");
    state.update(0);
    state.update(button::Up);
    require(state.selectedChannel() == 0, "Up cannot focus a planned channel");
    state.update(0);
    state.update(button::Down);
    require(state.selectedChannel() == 0, "Down cannot focus a planned channel");
    state.update(0);
    state.update(button::Cross);
    require(state.view() == View::Home, "Cross cannot open a planned channel");
    state.openChannel(channelIndex(ChannelId::Games));
    require(state.view() == View::Home, "direct Games open is rejected while planned");
    state.openChannel(channelIndex(ChannelId::Media));
    require(state.view() == View::Home, "direct Media open is rejected while planned");
    state.openChannel(channelCount());
    require(state.view() == View::Home, "out-of-range channel open is rejected");

    state.update(0);
    state.update(button::Start);
    require(state.view() == View::Menu, "Start opens menu");
    state.update(0);
    state.update(button::Down);
    require(state.selectedMenuItem() == 1, "menu focus moves");
    state.update(0);
    state.update(button::Cross);
    require(state.view() == View::About, "About menu item opens About");
    // Reopening the menu over About must preserve About's original Home parent.
    state.update(0);
    state.update(button::Start);
    require(state.view() == View::Menu, "Start can open the menu over About");
    state.update(0);
    state.update(button::Up);
    require(state.selectedMenuItem() == 0, "Up returns focus to Resume");
    state.update(0);
    state.update(button::Cross);
    require(state.view() == View::About, "Resume restores About rather than skipping to Home");
    state.update(0);
    state.update(button::Circle);
    require(state.view() == View::Home, "About returns to its original Home parent");

    // Back wins over Cross, so closing the menu cannot activate Exit in the
    // same controller sample.
    UiState chord;
    chord.update(button::Start);
    chord.update(0);
    chord.update(button::Up); // Exit
    require(chord.selectedMenuItem() == 2, "menu focus wraps to Exit");
    chord.update(0);
    chord.update(button::Circle | button::Cross);
    require(chord.view() == View::Home, "Circle closes the menu when Back and Cross are pressed together");
    require(!chord.shouldQuit(), "Back/Cross chord does not accidentally exit");

    chord.update(0);
    chord.update(button::Start);
    chord.update(0);
    chord.activateMenuItem(2);
    require(chord.shouldQuit(), "Exit menu item requests clean quit");

    std::cout << "PSBBV UI state: all checks passed.\n";
}
