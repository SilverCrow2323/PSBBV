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
            menuReturnView_ = view_;
            view_ = View::Menu;
        }
        return;
    }

    if (view_ == View::Menu) {
        // Back is the menu's highest-priority action. A Cross chord must not
        // activate the item that was focused while the menu is being closed.
        if ((pressed & button::Circle) != 0) {
            closeMenu();
            return;
        }

        // Opposing directions in one sample cancel rather than arbitrarily
        // selecting whichever branch runs last.
        const bool up = (pressed & button::Up) != 0;
        const bool down = (pressed & button::Down) != 0;
        if (up != down)
            selectedMenuItem_ = up ? (selectedMenuItem_ + 2) % 3
                                   : (selectedMenuItem_ + 1) % 3;
        if ((pressed & button::Cross) != 0) activateFocusedMenuItem();
        return;
    }

    if (view_ == View::About) {
        if ((pressed & (button::Circle | button::Cross)) != 0)
            view_ = aboutReturnView_;
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
    const bool up = (pressed & button::Up) != 0;
    const bool down = (pressed & button::Down) != 0;
    if (up != down)
        selectedChannel_ = up ? (selectedChannel_ + count - 1) % count
                              : (selectedChannel_ + 1) % count;
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
    else if (view_ == View::About) view_ = aboutReturnView_;
    else if (view_ == View::Channel) view_ = View::Home;
}

void UiState::closeMenu() noexcept {
    view_ = menuReturnView_;
}

void UiState::activateFocusedMenuItem() noexcept {
    switch (static_cast<MenuItem>(selectedMenuItem_)) {
        case MenuItem::Resume:
            closeMenu();
            break;
        case MenuItem::About:
            // Reopening About from its own menu must preserve the original
            // Home/Channel destination rather than making About its own parent.
            if (menuReturnView_ != View::About)
                aboutReturnView_ = menuReturnView_;
            view_ = View::About;
            break;
        case MenuItem::Exit:
            shouldQuit_ = true;
            break;
    }
}

} // namespace psbbv::navigation
