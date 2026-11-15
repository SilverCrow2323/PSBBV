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

class UiState final {
public:
    UiState() noexcept = default;

    void update(std::uint32_t heldButtons) noexcept;
    void openChannel(std::size_t index) noexcept;
    void activateMenuItem(std::size_t index) noexcept;
    void back() noexcept;

    [[nodiscard]] View view() const noexcept { return view_; }
    [[nodiscard]] std::size_t selectedChannel() const noexcept { return selectedChannel_; }
    [[nodiscard]] std::size_t selectedMenuItem() const noexcept { return selectedMenuItem_; }
    [[nodiscard]] bool shouldQuit() const noexcept { return shouldQuit_; }

private:
    void closeMenu() noexcept;
    void activateFocusedMenuItem() noexcept;

    std::uint32_t previousButtons_{0};
    View view_{View::Home};
    View returnView_{View::Home};
    std::size_t selectedChannel_{0};
    std::size_t selectedMenuItem_{0};
    bool shouldQuit_{false};
};

} // namespace psbbv::navigation
