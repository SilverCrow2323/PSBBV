#pragma once

#include <cstdint>

namespace psbbv::navigation {

// Convert Vita touch-panel coordinates into a screen pixel coordinate.
// Invalid panel ranges or screen extents return -1; valid inputs are clamped.
inline int mapTouchCoordinate(
    int value,
    int activeMin,
    int activeMax,
    int screenExtent
) noexcept {
    if (activeMax <= activeMin || screenExtent <= 0) {
        return -1;
    }
    if (screenExtent == 1) {
        return 0;
    }

    const int clamped = value < activeMin ? activeMin
        : value > activeMax ? activeMax
        : value;
    const std::int64_t span =
        static_cast<std::int64_t>(activeMax) - activeMin;
    const std::int64_t offset =
        static_cast<std::int64_t>(clamped) - activeMin;
    const std::int64_t scaled =
        offset * static_cast<std::int64_t>(screenExtent - 1);
    return static_cast<int>(scaled / span);
}

} // namespace psbbv::navigation
