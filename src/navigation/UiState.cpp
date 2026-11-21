#include "psbbv/navigation/UiState.hpp"

#include "psbbv/navigation/ChannelRegistry.hpp"

namespace psbbv::navigation {
namespace {

bool isImplementedChannel(std::size_t index) noexcept {
    const ChannelDefinition* channel = channelAt(index);
    return channel != nullptr &&
           channel->availability == ChannelAvailability::Implemented;
}

std::size_t firstImplementedChannel() noexcept {
    for (std::size_t i = 0; i < channelCount(); ++i) {
        if (isImplementedChannel(i)) return i;
    }
    return channelCount();
}

} // namespace

std::uint32_t UiState::directionPulse(std::uint32_t heldButtons) noexcept {
    const std::uint32_t direction = heldButtons & (button::Up | button::Down);
    if (direction != button::Up && direction != button::Down) {
        // Releasing both directions, or holding both, cancels the repeat clock.
        heldDirection_ = 0;
        directionHoldFrames_ = 0;
        directionRepeatFrames_ = 0;
        return 0;
    }

    if (direction != heldDirection_) {
        heldDirection_ = direction;
        directionHoldFrames_ = 0;
        directionRepeatFrames_ = 0;
        return direction;
    }

    if (directionHoldFrames_ < DirectionRepeatDelayFrames) {
        ++directionHoldFrames_;
        if (directionHoldFrames_ == DirectionRepeatDelayFrames) return direction;
        return 0;
    }

    ++directionRepeatFrames_;
    if (directionRepeatFrames_ >= DirectionRepeatIntervalFrames) {
        directionRepeatFrames_ = 0;
        return direction;
    }
    return 0;
}

void UiState::update(std::uint32_t heldButtons) noexcept {
    const std::uint32_t pressed = heldButtons & ~previousButtons_;
    previousButtons_ = heldButtons;

    if (shouldQuit_) return;

    if ((pressed & button::Start) != 0) {
        // Start changes the focused surface. A held direction may continue,
        // but its repeat delay must restart instead of leaking across views.
        const std::uint32_t heldDirection = heldButtons & (button::Up | button::Down);
        heldDirection_ = (heldDirection == button::Up || heldDirection == button::Down)
            ? heldDirection
            : 0;
        directionHoldFrames_ = 0;
        directionRepeatFrames_ = 0;
        if (view_ == View::Menu) closeMenu();
        else {
            menuReturnView_ = view_;
            view_ = View::Menu;
        }
        return;
    }

    const std::uint32_t direction = directionPulse(heldButtons);

    if (view_ == View::Menu) {
        // Back is the menu's highest-priority action. A Cross chord must not
        // activate the item that was focused while the menu is being closed.
        if ((pressed & button::Circle) != 0) {
            closeMenu();
            return;
        }

        // Opposing directions in one sample cancel rather than arbitrarily
        // selecting whichever branch runs last.
        if (direction == button::Up)
            selectedMenuItem_ = (selectedMenuItem_ + 2) % 3;
        else if (direction == button::Down)
            selectedMenuItem_ = (selectedMenuItem_ + 1) % 3;
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
    const std::size_t first = firstImplementedChannel();
    if (first >= count) {
        // Planned destinations remain visible as roadmap metadata, but they
        // are not focusable or openable until their real implementation lands.
        selectedChannel_ = 0;
        return;
    }

    if (!isImplementedChannel(selectedChannel_)) selectedChannel_ = first;

    if (direction == button::Up || direction == button::Down) {
        const bool up = direction == button::Up;
        std::size_t candidate = selectedChannel_;
        for (std::size_t visited = 0; visited < count; ++visited) {
            candidate = up ? (candidate + count - 1) % count
                           : (candidate + 1) % count;
            if (isImplementedChannel(candidate)) {
                selectedChannel_ = candidate;
                break;
            }
        }
    }

    if ((pressed & button::Cross) != 0 && isImplementedChannel(selectedChannel_))
        view_ = View::Channel;
}

void UiState::openChannel(std::size_t index) noexcept {
    if (!isImplementedChannel(index)) return;
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
