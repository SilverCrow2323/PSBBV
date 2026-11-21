#include "psbbv/navigation/GamesBrowserState.hpp"

namespace psbbv::navigation {

void GamesBrowserState::setItemCount(std::size_t count) noexcept {
    itemCount_ = count;
    if (itemCount_ == 0) {
        selectedIndex_ = 0;
        firstVisibleIndex_ = 0;
        return;
    }
    if (selectedIndex_ >= itemCount_) selectedIndex_ = itemCount_ - 1;
    keepSelectionVisible();
}

void GamesBrowserState::moveUp() noexcept {
    if (itemCount_ == 0) return;
    selectedIndex_ = selectedIndex_ == 0 ? itemCount_ - 1 : selectedIndex_ - 1;
    keepSelectionVisible();
}

void GamesBrowserState::moveDown() noexcept {
    if (itemCount_ == 0) return;
    selectedIndex_ = (selectedIndex_ + 1) % itemCount_;
    keepSelectionVisible();
}

void GamesBrowserState::select(std::size_t index) noexcept {
    if (index >= itemCount_) return;
    selectedIndex_ = index;
    keepSelectionVisible();
}

std::size_t GamesBrowserState::visibleCount() const noexcept {
    if (firstVisibleIndex_ >= itemCount_) return 0;
    const std::size_t remaining = itemCount_ - firstVisibleIndex_;
    return remaining < VisibleRows ? remaining : VisibleRows;
}

void GamesBrowserState::keepSelectionVisible() noexcept {
    if (selectedIndex_ < firstVisibleIndex_) {
        firstVisibleIndex_ = selectedIndex_;
    } else if (selectedIndex_ >= firstVisibleIndex_ + VisibleRows) {
        firstVisibleIndex_ = selectedIndex_ - VisibleRows + 1;
    }

    const std::size_t maxFirst = itemCount_ > VisibleRows
        ? itemCount_ - VisibleRows
        : 0;
    if (firstVisibleIndex_ > maxFirst) firstVisibleIndex_ = maxFirst;
}

} // namespace psbbv::navigation
