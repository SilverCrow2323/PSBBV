#pragma once

#include <cstddef>

namespace psbbv::navigation {

class GamesBrowserState final {
public:
    static constexpr std::size_t VisibleRows = 5;

    void setItemCount(std::size_t count) noexcept;
    void moveUp() noexcept;
    void moveDown() noexcept;
    void select(std::size_t index) noexcept;

    [[nodiscard]] std::size_t itemCount() const noexcept { return itemCount_; }
    [[nodiscard]] std::size_t selectedIndex() const noexcept { return selectedIndex_; }
    [[nodiscard]] std::size_t firstVisibleIndex() const noexcept { return firstVisibleIndex_; }
    [[nodiscard]] std::size_t visibleCount() const noexcept;

private:
    void keepSelectionVisible() noexcept;

    std::size_t itemCount_{0};
    std::size_t selectedIndex_{0};
    std::size_t firstVisibleIndex_{0};
};

} // namespace psbbv::navigation
