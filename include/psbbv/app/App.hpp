#pragma once

#include <cstddef>
#include <cstdint>

#include <vita2d.h>

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
    void activateMenuItem() noexcept;
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
    std::uint32_t previousButtons_;
    std::size_t selectedChannel_;
    std::size_t selectedMenuItem_;
    bool channelOpen_;
    bool applicationMenuOpen_;
    bool aboutOpen_;
    bool shouldQuit_;
};

} // namespace psbbv
