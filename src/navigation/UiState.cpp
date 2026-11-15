#include "psbbv/navigation/UiState.hpp"

#include "psbbv/navigation/ChannelRegistry.hpp"

namespace psbbv::navigation {

void UiState::update(std::uint32_t heldButtons) noexcept {
    const std::uint32_t pressed = heldButtons & ~previousButtons_;
    previousButtons_ = heldButtons;

    if (shouldQuit_) return;

    if ((pressed & button::Start) != 0) {
        if (view_ == View::Menu) closeMenu();
        else {
            returnView_ = view_;
            view_ = View::Menu;
        }
        return;
    }

    if (view_ == View::Menu) {
        if ((pressed & button::Up) != 0)
            selectedMenuItem_ = (selectedMenuItem_ + 2) % 3;
        if ((pressed & button::Down) != 0)
            selectedMenuItem_ = (selectedMenuItem_ + 1) % 3;
        if ((pressed & button::Circle) != 0) closeMenu();
        if ((pressed & button::Cross) != 0) activateFocusedMenuItem();
        return;
    }

    if (view_ == View::About) {
        if ((pressed & (button::Circle | button::Cross)) != 0)
            view_ = returnView_;
        return;
    }

    if (view_ == View::Channel) {
        if ((pressed & button::Circle) != 0) view_ = View::Home;
        return;
    }

    const std::size_t count = channelCount();
    if (count == 0) {
        selectedChannel_ = 0;
        return;
    }
    if ((pressed & button::Up) != 0)
        selectedChannel_ = (selectedChannel_ + count - 1) % count;
    if ((pressed & button::Down) != 0)
        selectedChannel_ = (selectedChannel_ + 1) % count;
    if ((pressed & button::Cross) != 0) view_ = View::Channel;
}

void UiState::openChannel(std::size_t index) noexcept {
    if (index >= channelCount()) return;
    selectedChannel_ = index;
    view_ = View::Channel;
}

void UiState::activateMenuItem(std::size_t index) noexcept {
    if (index >= 3 || view_ != View::Menu) return;
    selectedMenuItem_ = index;
    activateFocusedMenuItem();
}

void UiState::back() noexcept {
    if (view_ == View::Menu) closeMenu();
    else if (view_ == View::About) view_ = returnView_;
    else if (view_ == View::Channel) view_ = View::Home;
}

void UiState::closeMenu() noexcept {
    view_ = returnView_;
}

void UiState::activateFocusedMenuItem() noexcept {
    switch (static_cast<MenuItem>(selectedMenuItem_)) {
        case MenuItem::Resume:
            closeMenu();
            break;
        case MenuItem::About:
            view_ = View::About;
            break;
        case MenuItem::Exit:
            shouldQuit_ = true;
            break;
    }
}

} // namespace psbbv::navigation
