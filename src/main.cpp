#include "psbbv/navigation.hpp"
#include <psp2/ctrl.h>
#include <psp2/kernel/processmgr.h>
#include <vita2d.h>
#include <cstdint>

namespace {
constexpr int ScreenWidth = 960;
constexpr int ScreenHeight = 544;
constexpr std::uint32_t White = RGBA8(245, 246, 250, 255);
constexpr std::uint32_t Muted = RGBA8(151, 164, 184, 255);
constexpr std::uint32_t Accent = RGBA8(220, 48, 57, 255);
constexpr std::uint32_t Panel = RGBA8(24, 29, 39, 245);
constexpr std::uint32_t Background = RGBA8(10, 13, 20, 255);
void drawText(vita2d_pgf* font, float x, float y, std::uint32_t color, float size, const char* value) {
    vita2d_pgf_draw_text(font, x, y, color, size, value);
}
void drawMenu(vita2d_pgf* font, const psbbv::NavigationState& state) {
    vita2d_draw_rectangle(0, 0, ScreenWidth, ScreenHeight, Background);
    drawText(font, 58, 72, Accent, 30.0f, "PSBBV");
    drawText(font, 58, 105, Muted, 17.0f, "PLAYSTATION BROADBAND VITA");
    vita2d_draw_rectangle(58, 132, 844, 2, RGBA8(62, 70, 86, 255));
    for (std::size_t i = 0; i < psbbv::Channels.size(); ++i) {
        const float y = 164.0f + static_cast<float>(i) * 55.0f;
        const bool selected = i == state.selected;
        if (selected) {
            vita2d_draw_rectangle(58, y - 32, 844, 46, Panel);
            vita2d_draw_rectangle(58, y - 32, 5, 46, Accent);
        }
        drawText(font, 84, y, selected ? White : Muted, 22.0f, psbbv::Channels[i].title);
        drawText(font, 390, y, Muted, 16.0f, psbbv::Channels[i].subtitle);
    }
    vita2d_draw_rectangle(58, 500, 844, 1, RGBA8(62, 70, 86, 255));
    drawText(font, 58, 526, Muted, 14.0f, "D-PAD  NAVIGATE     CROSS  OPEN     START  EXIT");
}
void drawChannel(vita2d_pgf* font, const psbbv::NavigationState& state) {
    const auto& channel = psbbv::selectedChannel(state);
    vita2d_draw_rectangle(0, 0, ScreenWidth, ScreenHeight, Background);
    drawText(font, 58, 72, Accent, 28.0f, "PSBBV");
    drawText(font, 58, 160, White, 34.0f, channel.title);
    drawText(font, 58, 198, Muted, 18.0f, channel.subtitle);
    vita2d_draw_rectangle(58, 230, 844, 2, RGBA8(62, 70, 86, 255));
    drawText(font, 58, 290, White, 20.0f, "CHANNEL FOUNDATION");
    drawText(font, 58, 326, Muted, 17.0f, "This channel is a navigation placeholder. Its domain module is not wired yet.");
    drawText(font, 58, 526, Muted, 14.0f, "CIRCLE  BACK     START  EXIT");
}
}
int main() {
    sceCtrlSetSamplingMode(SCE_CTRL_MODE_ANALOG);
    vita2d_init();
    vita2d_pgf* font = vita2d_load_default_pgf();
    if (font == nullptr) { vita2d_fini(); sceKernelExitProcess(1); return 1; }
    psbbv::NavigationState state{};
    std::uint32_t previousButtons = 0;
    bool running = true;
    while (running) {
        SceCtrlData pad{};
        sceCtrlPeekBufferPositive(0, &pad, 1);
        const std::uint32_t pressed = pad.buttons & ~previousButtons;
        previousButtons = pad.buttons;
        if ((pressed & SCE_CTRL_START) != 0) running = false;
        else if (state.channel_open) {
            if ((pressed & SCE_CTRL_CIRCLE) != 0) psbbv::goBack(state);
        } else {
            if ((pressed & SCE_CTRL_UP) != 0) psbbv::moveSelection(state, -1);
            if ((pressed & SCE_CTRL_DOWN) != 0) psbbv::moveSelection(state, 1);
            if ((pressed & SCE_CTRL_CROSS) != 0) psbbv::openSelected(state);
        }
        vita2d_start_drawing();
        vita2d_clear_screen();
        if (state.channel_open) drawChannel(font, state); else drawMenu(font, state);
        vita2d_end_drawing();
        vita2d_swap_buffers();
        sceKernelDelayThread(16000);
    }
    vita2d_fini();
    sceKernelExitProcess(0);
    return 0;
}
