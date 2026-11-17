#pragma once

#include <cstddef>

namespace psbbv::navigation {

constexpr std::size_t MenuItemCount = 3;
constexpr std::size_t NoMenuItem = MenuItemCount;

constexpr int MenuTouchLeft = 320;
constexpr int MenuTouchTop = 238;
constexpr int MenuTouchWidth = 320;
constexpr int MenuTouchRowPitch = 48;
constexpr int MenuTouchRowHeight = 40;

// Match the visible menu rectangles exactly. The eight-pixel visual gaps are
// intentionally not clickable, so a tap cannot activate the next row by
// landing between two menu items.
[[nodiscard]] constexpr std::size_t menuItemAtTouch(int x, int y) noexcept {
    if (x < MenuTouchLeft || x >= MenuTouchLeft + MenuTouchWidth ||
        y < MenuTouchTop) {
        return NoMenuItem;
    }

    const int offset = y - MenuTouchTop;
    if (offset >= MenuTouchRowPitch * static_cast<int>(MenuItemCount)) {
        return NoMenuItem;
    }

    const int rowOffset = offset % MenuTouchRowPitch;
    if (rowOffset >= MenuTouchRowHeight) {
        return NoMenuItem;
    }

    return static_cast<std::size_t>(offset / MenuTouchRowPitch);
}

} // namespace psbbv::navigation
