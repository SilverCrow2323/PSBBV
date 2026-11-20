#include "psbbv/navigation/TouchMapping.hpp"

#include <cstdlib>
#include <iostream>
#include <limits>

namespace {
void require(bool ok, const char* message) {
    if (!ok) {
        std::cerr << "FAIL: " << message << '\n';
        std::exit(EXIT_FAILURE);
    }
}
}

int main() {
    using psbbv::navigation::mapTouchCoordinate;

    require(mapTouchCoordinate(0, 0, 100, 960) == 0,
            "panel minimum maps to first pixel");
    require(mapTouchCoordinate(100, 0, 100, 960) == 959,
            "panel maximum maps to last pixel");
    require(mapTouchCoordinate(50, 0, 100, 960) == 479,
            "midpoint uses deterministic floor scaling");
    require(mapTouchCoordinate(-10, 0, 100, 960) == 0,
            "coordinates below panel range clamp to minimum");
    require(mapTouchCoordinate(110, 0, 100, 960) == 959,
            "coordinates above panel range clamp to maximum");
    require(mapTouchCoordinate(42, 10, 10, 960) == -1,
            "zero-width panel range is rejected");
    require(mapTouchCoordinate(42, 20, 10, 960) == -1,
            "inverted panel range is rejected");
    require(mapTouchCoordinate(42, 0, 100, 0) == -1,
            "zero screen extent is rejected");
    require(mapTouchCoordinate(42, 0, 100, 1) == 0,
            "single-pixel extent is handled");
    require(mapTouchCoordinate(std::numeric_limits<int>::min(),
                              std::numeric_limits<int>::min(),
                              std::numeric_limits<int>::max(), 960) == 0,
            "wide signed panel range maps its minimum safely");
    require(mapTouchCoordinate(0,
                              std::numeric_limits<int>::min(),
                              std::numeric_limits<int>::max(), 960) == 479,
            "wide signed panel range scales without integer overflow");
    require(mapTouchCoordinate(std::numeric_limits<int>::max(),
                              std::numeric_limits<int>::min(),
                              std::numeric_limits<int>::max(), 960) == 959,
            "wide signed panel range maps its maximum safely");

    std::cout << "PSBBV touch coordinate mapping: all checks passed.\\n";
}
