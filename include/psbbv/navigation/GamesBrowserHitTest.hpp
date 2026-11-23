#pragma once

#include <cstddef>

#include "psbbv/navigation/GamesBrowserState.hpp"

namespace psbbv::navigation {

constexpr std::size_t NoGamesBrowserItem = static_cast<std::size_t>(-1);
constexpr int GamesBrowserTouchLeft = 305;
constexpr int GamesBrowserTouchRight = 889;
constexpr int GamesBrowserFirstRowTop = 302;
constexpr int GamesBrowserRowHeight = 27;
constexpr int GamesBrowserRowHitHeight = 24;

// Match the visible row rectangle, not the three-pixel visual gap between rows.
// The final partial page exposes only the records that actually exist.
inline std::size_t gamesBrowserItemAtTouch(
    int x,
    int y,
    std::size_t firstVisibleIndex,
    std::size_t itemCount
) noexcept {
    if (itemCount == 0 || firstVisibleIndex >= itemCount ||
        x < GamesBrowserTouchLeft || x > GamesBrowserTouchRight ||
        y < GamesBrowserFirstRowTop) {
        return NoGamesBrowserItem;
    }

    const std::size_t offset = static_cast<std::size_t>(y - GamesBrowserFirstRowTop);
    const std::size_t row = offset / static_cast<std::size_t>(GamesBrowserRowHeight);
    const std::size_t rowOffset = offset % static_cast<std::size_t>(GamesBrowserRowHeight);

    if (row >= GamesBrowserState::VisibleRows ||
        rowOffset >= static_cast<std::size_t>(GamesBrowserRowHitHeight) ||
        row >= itemCount - firstVisibleIndex) {
        return NoGamesBrowserItem;
    }

    return firstVisibleIndex + row;
}

} // namespace psbbv::navigation
