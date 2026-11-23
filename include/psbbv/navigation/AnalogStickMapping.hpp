#pragma once

#include <cstdint>

#include "psbbv/navigation/UiState.hpp"

namespace psbbv::navigation {

constexpr std::uint8_t AnalogAxisCenter = 128;
constexpr std::uint8_t AnalogAxisDeadzone = 48;
constexpr std::uint8_t AnalogAxisUpThreshold =
    static_cast<std::uint8_t>(AnalogAxisCenter - AnalogAxisDeadzone);
constexpr std::uint8_t AnalogAxisDownThreshold =
    static_cast<std::uint8_t>(AnalogAxisCenter + AnalogAxisDeadzone);

// Vita analog axes rest at 128. Values inside the deadzone are neutral;
// outside it, vertical stick movement is translated into the existing D-pad
// path so focus timing and edge handling stay consistent.
[[nodiscard]] constexpr std::uint32_t verticalAnalogDirection(
    std::uint8_t axis
) noexcept {
    if (axis < AnalogAxisUpThreshold) return button::Up;
    if (axis > AnalogAxisDownThreshold) return button::Down;
    return 0;
}

[[nodiscard]] constexpr std::uint32_t mergeControllerInput(
    std::uint32_t digitalButtons,
    std::uint8_t analogY
) noexcept {
    return digitalButtons | verticalAnalogDirection(analogY);
}

} // namespace psbbv::navigation
