#pragma once

#include <cstddef>
#include <cstdint>

namespace psbbv::navigation {

enum class View : std::uint8_t { Home, Channel, About, Menu };
enum class MenuItem : std::uint8_t { Resume, About, Exit };

namespace button {
constexpr std::uint32_t Up = 0x00000010;
constexpr std::uint32_t Down = 0x00000040;
constexpr std::uint32_t Start = 0x00000008;
constexpr std::uint32_t Circle = 0x00002000;
constexpr std::uint32_t Cross = 0x00004000;
} // namespace button

// The Vita frame loop waits for vblank. These values therefore describe
// deterministic 60 Hz navigation timing rather than wall-clock time.
constexpr std::uint16_t DirectionRepeatDelayFrames = 18;
constexpr std::uint16_t DirectionRepeatIntervalFrames = 5;

class UiState final {
public:
    UiState() noexcept = default;

    void update(std::uint32_t heldButtons) noexcept;
    void openChannel(std::size_t index) noexcept;
    void activateMenuItem(std::size_t index) noexcept;
    void back() noexcept;

    [[nodiscard]] View view() const noexcept { return view_; }
    [[nodiscard]] View backgroundView() const noexcept { return menuReturnView_; }
    [[nodiscard]] std::size_t selectedChannel() const noexcept { return selectedChannel_; }
    [[nodiscard]] std::size_t selectedMenuItem() const noexcept { return selectedMenuItem_; }
    [[nodiscard]] bool shouldQuit() const noexcept { return shouldQuit_; }

private:
    void closeMenu() noexcept;
    void activateFocusedMenuItem() noexcept;
    [[nodiscard]] std::uint32_t directionPulse(std::uint32_t heldButtons) noexcept;

    std::uint32_t previousButtons_{0};
    std::uint32_t heldDirection_{0};
    std::uint16_t directionHoldFrames_{0};
    std::uint16_t directionRepeatFrames_{0};
    View view_{View::Home};
    // Keep the menu's background and About's back destination independent.
    // Opening the menu while About is visible must not overwrite About's origin.
    View menuReturnView_{View::Home};
    View aboutReturnView_{View::Home};
    std::size_t selectedChannel_{0};
    std::size_t selectedMenuItem_{0};
    bool shouldQuit_{false};
};

} // namespace psbbv::navigation
