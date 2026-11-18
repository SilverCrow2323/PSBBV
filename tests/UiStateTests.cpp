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

    UiState state;

    state.update(button::Up | button::Down);
    require(state.selectedChannel() == 0, "opposing directions cancel in one input sample");
    state.update(0);
    state.update(button::Up);
    require(state.selectedChannel() == channelCount() - 1, "Up wraps from first to last channel");
    state.update(0);
    state.update(button::Down);
    require(state.selectedChannel() == 0, "Down wraps from last to first channel");
    state.update(0);
    state.update(button::Down);
    require(state.selectedChannel() == 1, "Down moves focus once");
    state.update(button::Down);
    require(state.selectedChannel() == 1, "held button does not retrigger");
    state.update(0);
    state.update(button::Down);
    require(state.selectedChannel() == 2, "released button can trigger again");

    state.update(button::Start);
    require(state.view() == View::Menu, "Start opens menu");
    state.update(0);
    state.update(button::Down);
    require(state.selectedMenuItem() == 1, "menu focus moves");
    state.update(0);
    state.update(button::Cross);
    require(state.view() == View::About, "About menu item opens About");
    state.update(0);
    state.update(button::Circle);
    require(state.view() == View::Home, "About returns to prior Home view");

    state.openChannel(channelIndex(ChannelId::Media));
    require(state.view() == View::Channel, "channel opens");
    require(state.selectedChannel() == channelIndex(ChannelId::Media), "opened channel receives focus");
    state.update(0);
    state.update(button::Start);
    require(state.view() == View::Menu, "Start opens menu over channel");
    state.update(0);
    state.update(button::Circle);
    require(state.view() == View::Channel, "closing menu restores channel view");
    state.update(0);
    state.update(button::Circle);
    require(state.view() == View::Home, "Circle returns from channel");

    // Regression: opening the menu while About is visible must not replace
    // About's original destination with About itself.
    state.openChannel(channelIndex(ChannelId::Games));
    state.update(0);
    state.update(button::Start);
    state.update(0);
    state.update(button::Cross); // selected About item
    require(state.view() == View::About, "About opens from a channel");
    state.update(0);
    state.update(button::Start);
    require(state.view() == View::Menu, "Start can open the menu over About");
    state.update(0);
    state.update(button::Up); // Resume
    state.update(0);
    state.update(button::Cross);
    require(state.view() == View::About, "Resume restores About");
    state.update(0);
    state.update(button::Circle);
    require(state.view() == View::Channel, "About back returns to its original channel after menu resume");

    // Regression: Back wins over Cross, so closing the menu cannot activate
    // the focused About or Exit action in the same controller sample.
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
    chord.update(button::Cross);
    require(chord.view() == View::About, "Cross still activates About when used alone");

    state.update(0);
    state.update(button::Start);
    state.update(0);
    state.activateMenuItem(2);
    require(state.shouldQuit(), "Exit menu item requests clean quit");

    std::cout << "PSBBV UI state: all checks passed.\n";
}
