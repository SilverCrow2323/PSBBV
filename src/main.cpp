#include <algorithm>
#include <cstring>

#include <psp2/ctrl.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/touch.h>

#include <vita2d.h>

#include "psbbv/app/App.hpp"

namespace {

constexpr int ScreenWidth = 960;
constexpr int ScreenHeight = 544;

int mapTouchCoordinate(
    int value,
    int activeMin,
    int activeMax,
    int screenExtent
) noexcept {
    if (activeMax <= activeMin) {
        return -1;
    }

    const int clamped = std::clamp(value, activeMin, activeMax);
    const int numerator = (clamped - activeMin) * (screenExtent - 1);
    return numerator / (activeMax - activeMin);
}

} // namespace

int main(int, char**) {
    if (vita2d_init() < 0) {
        sceKernelExitProcess(1);
        return 1;
    }

    vita2d_set_vblank_wait(1);
    vita2d_set_clear_color(RGBA8(8, 10, 14, 255));

    vita2d_pgf* font = vita2d_load_default_pgf();
    psbbv::App app(font);

    SceTouchPanelInfo frontPanel{};
    const bool touchReady =
        sceTouchGetPanelInfo(SCE_TOUCH_PORT_FRONT, &frontPanel) >= 0 &&
        sceTouchSetSamplingState(
            SCE_TOUCH_PORT_FRONT,
            SCE_TOUCH_SAMPLING_STATE_START
        ) >= 0;

    SceTouchData previousTouch{};
    SceCtrlData controller{};

    while (!app.shouldQuit()) {
        sceCtrlPeekBufferPositive(0, &controller, 1);

        int touchX = -1;
        int touchY = -1;

        if (touchReady) {
            SceTouchData currentTouch{};
            if (sceTouchPeek(SCE_TOUCH_PORT_FRONT, &currentTouch, 1) >= 0) {
                const bool justPressed =
                    currentTouch.reportNum > 0 && previousTouch.reportNum == 0;

                if (justPressed) {
                    touchX = mapTouchCoordinate(
                        currentTouch.report[0].x,
                        frontPanel.minAaX,
                        frontPanel.maxAaX,
                        ScreenWidth
                    );
                    touchY = mapTouchCoordinate(
                        currentTouch.report[0].y,
                        frontPanel.minAaY,
                        frontPanel.maxAaY,
                        ScreenHeight
                    );
                }

                previousTouch = currentTouch;
            }
        }

        app.update(controller.buttons, touchX, touchY);

        vita2d_start_drawing();
        vita2d_clear_screen();
        app.render();
        vita2d_end_drawing();
        vita2d_swap_buffers();
    }

    vita2d_wait_rendering_done();

    if (font != nullptr) {
        vita2d_free_pgf(font);
    }

    if (touchReady) {
        sceTouchSetSamplingState(
            SCE_TOUCH_PORT_FRONT,
            SCE_TOUCH_SAMPLING_STATE_STOP
        );
    }

    vita2d_fini();
    sceKernelExitProcess(0);
    return 0;
}
