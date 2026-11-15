#pragma once

#include <cstddef>
#include <cstdint>

#include <vita2d.h>

#include "psbbv/navigation/UiState.hpp"

namespace psbbv {

class App final {
public:
    explicit App(vita2d_pgf* font) noexcept;

    void update(
        std::uint32_t buttons,
        int touchX,
        int touchY
    ) noexcept;

    void render() const noexcept;

    [[nodiscard]] bool shouldQuit() const noexcept;

private:
    void handleTouch(int x, int y) noexcept;

    void drawText(
        int x,
        int y,
        unsigned int color,
        float scale,
        const char* text
    ) const noexcept;

    void renderHeader() const noexcept;
    void renderNavigation() const noexcept;
    void renderHome() const noexcept;
    void renderChannel() const noexcept;
    void renderAbout() const noexcept;
    void renderApplicationMenu() const noexcept;
    void renderFooter() const noexcept;

    vita2d_pgf* font_;
    navigation::UiState state_;
    bool touchWasDown_;
};

} // namespace psbbv
