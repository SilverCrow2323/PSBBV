#include "psbbv/navigation/GamesBrowserHitTest.hpp"

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
    using namespace psbbv::navigation;

    require(gamesBrowserItemAtTouch(305, 302, 0, 12) == 0,
            "top-left visible row boundary selects the first item");
    require(gamesBrowserItemAtTouch(889, 325, 0, 12) == 0,
            "right and bottom visible pixels remain inside the first row");
    require(gamesBrowserItemAtTouch(304, 310, 0, 12) == NoGamesBrowserItem,
            "touch left of the visible row is rejected");
    require(gamesBrowserItemAtTouch(890, 310, 0, 12) == NoGamesBrowserItem,
            "touch right of the visible row is rejected");
    require(gamesBrowserItemAtTouch(400, 301, 0, 12) == NoGamesBrowserItem,
            "touch above the first row is rejected");

    require(gamesBrowserItemAtTouch(400, 326, 0, 12) == NoGamesBrowserItem,
            "the first three-pixel visual gap is not clickable");
    require(gamesBrowserItemAtTouch(400, 328, 0, 12) == NoGamesBrowserItem,
            "the last pixel of the first visual gap is not clickable");
    require(gamesBrowserItemAtTouch(400, 329, 0, 12) == 1,
            "the next row starts at its own visible top edge");

    require(gamesBrowserItemAtTouch(400, 433, 0, 12) == 4,
            "the fifth row accepts its final visible pixel");
    require(gamesBrowserItemAtTouch(400, 434, 0, 12) == NoGamesBrowserItem,
            "the fifth row visual gap is not clickable");
    require(gamesBrowserItemAtTouch(400, 437, 0, 12) == NoGamesBrowserItem,
            "touch below the five-row viewport is rejected");

    require(gamesBrowserItemAtTouch(400, 302, 4, 7) == 4,
            "partial final page maps its first visible row");
    require(gamesBrowserItemAtTouch(400, 329, 4, 7) == 5,
            "partial final page maps its second visible row");
    require(gamesBrowserItemAtTouch(400, 356, 4, 7) == 6,
            "partial final page maps its final existing item");
    require(gamesBrowserItemAtTouch(400, 383, 4, 7) == NoGamesBrowserItem,
            "empty rows after the final item cannot select stale entries");

    require(gamesBrowserItemAtTouch(400, 302, 7, 7) == NoGamesBrowserItem,
            "viewport starting beyond the catalogue is rejected");
    require(gamesBrowserItemAtTouch(400, 302, 0, 0) == NoGamesBrowserItem,
            "an empty catalogue has no touch target");

    std::cout << "PSBBV Games browser touch hit testing: all checks passed.\n";
    return EXIT_SUCCESS;
}
