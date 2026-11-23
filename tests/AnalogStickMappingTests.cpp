#include "psbbv/navigation/AnalogStickMapping.hpp"

#include <cassert>
#include <cstdint>

using namespace psbbv::navigation;

int main() {
    assert(verticalAnalogDirection(0) == button::Up);
    assert(verticalAnalogDirection(79) == button::Up);
    assert(verticalAnalogDirection(80) == 0);
    assert(verticalAnalogDirection(127) == 0);
    assert(verticalAnalogDirection(128) == 0);
    assert(verticalAnalogDirection(176) == 0);
    assert(verticalAnalogDirection(177) == button::Down);
    assert(verticalAnalogDirection(255) == button::Down);

    assert(mergeControllerInput(button::Cross, 128) == button::Cross);
    assert(mergeControllerInput(button::Up, 128) == button::Up);
    assert(mergeControllerInput(0, 0) == button::Up);
    assert(mergeControllerInput(0, 255) == button::Down);
    return 0;
}
