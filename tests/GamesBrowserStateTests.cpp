#include "psbbv/navigation/GamesBrowserState.hpp"

#include <cstdlib>
#include <iostream>

namespace {
void require(bool ok, const char* message) {
    if (!ok) {
        std::cerr << "FAIL: " << message << '\n';
        std::exit(EXIT_FAILURE);
    }
}
}

int main() {
    using psbbv::navigation::GamesBrowserState;
    GamesBrowserState browser;

    require(browser.itemCount() == 0, "browser starts empty");
    browser.moveUp();
    browser.moveDown();
    require(browser.selectedIndex() == 0 && browser.visibleCount() == 0,
            "empty catalogue ignores navigation safely");

    browser.setItemCount(12);
    require(browser.visibleCount() == GamesBrowserState::VisibleRows,
            "large catalogue exposes exactly five rows");
    require(browser.selectedIndex() == 0 && browser.firstVisibleIndex() == 0,
            "first title is initially focused");

    for (int i = 0; i < 5; ++i) browser.moveDown();
    require(browser.selectedIndex() == 5, "Down advances selection");
    require(browser.firstVisibleIndex() == 1,
            "viewport scrolls only when selection leaves the visible window");
    require(browser.visibleCount() == 5, "scrolled viewport remains full");

    browser.moveUp();
    require(browser.selectedIndex() == 4 && browser.firstVisibleIndex() == 0,
            "moving upward restores the previous viewport");

    browser.moveUp();
    require(browser.selectedIndex() == 11 && browser.firstVisibleIndex() == 7,
            "Up wraps to the final title and keeps it visible");
    browser.moveDown();
    require(browser.selectedIndex() == 0 && browser.firstVisibleIndex() == 0,
            "Down wraps from the final title to the first");

    browser.select(8);
    require(browser.selectedIndex() == 8 && browser.firstVisibleIndex() == 4,
            "touch selection scrolls the chosen title into view");
    browser.select(99);
    require(browser.selectedIndex() == 8,
            "out-of-range touch selection is ignored");

    browser.setItemCount(3);
    require(browser.selectedIndex() == 2 && browser.firstVisibleIndex() == 0 &&
            browser.visibleCount() == 3,
            "catalogue shrink clamps focus and removes stale scroll offset");
    browser.setItemCount(0);
    require(browser.selectedIndex() == 0 && browser.firstVisibleIndex() == 0 &&
            browser.visibleCount() == 0,
            "empty refresh resets focus and viewport");

    std::cout << "PSBBV Games browser state: all checks passed.\n";
}
