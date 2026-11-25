#include "psbbv/app/App.hpp"

#include <array>
#include <cstddef>

#include <psp2/ctrl.h>

#include <vita2d.h>

#include "psbbv/navigation/ChannelRegistry.hpp"
#include "psbbv/navigation/MenuHitTest.hpp"
#include "psbbv/navigation/UiState.hpp"
#include "psbbv/ui/Theme.hpp"

namespace psbbv {
namespace {

constexpr int ScreenWidth = 960;
constexpr int ScreenHeight = 544;

constexpr std::array<const char*, 3> MenuItems{{
    "RESUME NAVIGATION",
    "ABOUT PSBBV",
    "EXIT APPLICATION"
}};

} // namespace

App::App(vita2d_pgf* font) noexcept
    : font_(font),
      state_(),
      touchWasDown_(false) {}

App::~App() noexcept = default;

void App::shutdown() noexcept {}

void App::update(std::uint32_t buttons, int touchX, int touchY) noexcept {
    state_.update(buttons);
    const bool touchDown = touchX >= 0 && touchY >= 0;
    if (touchDown && !touchWasDown_) handleTouch(touchX, touchY);
    touchWasDown_ = touchDown;
}

bool App::shouldQuit() const noexcept { return state_.shouldQuit(); }

void App::handleTouch(int x, int y) noexcept {
    using navigation::View;
    if (state_.view() == View::Menu) {
        const std::size_t item = navigation::menuItemAtTouch(x, y);
        if (item != navigation::NoMenuItem) state_.activateMenuItem(item);
        else if (!navigation::menuPanelContainsTouch(x, y)) state_.back();
        return;
    }
    if (state_.view() == View::About) { state_.back(); return; }
    if (state_.view() == View::Channel) {
        if (y >= 480 && x <= 180) state_.back();
        return;
    }


}

void App::drawText(
    int x,
    int y,
    unsigned int color,
    float scale,
    const char* text
) const noexcept {
    if (font_ != nullptr && text != nullptr) {
        vita2d_pgf_draw_text(font_, x, y, color, scale, text);
    }
}

void App::render() const noexcept {
    using namespace ui::theme;

    vita2d_draw_rectangle(0, 0, ScreenWidth, ScreenHeight, Background);
    vita2d_draw_rectangle(0, 0, ScreenWidth, 4, AccentDim);
    vita2d_draw_rectangle(0, 0, 244, ScreenHeight, Sidebar);
    vita2d_draw_rectangle(243, 0, 1, ScreenHeight, Border);

    // Restrained diagonal accents add depth without competing with content.
    vita2d_draw_line(690, 4, 956, 270, RGBA8(37, 24, 31, 255));
    vita2d_draw_line(744, 4, 956, 216, RGBA8(29, 26, 34, 255));
    vita2d_draw_rectangle(0, 92, ScreenWidth, 1, Border);

    renderHeader();
    renderNavigation();

    const navigation::View visibleView = state_.view() == navigation::View::Menu
        ? state_.backgroundView()
        : state_.view();
    switch (visibleView) {
        case navigation::View::Home: renderHome(); break;
        case navigation::View::Channel: renderChannel(); break;
        case navigation::View::About: renderAbout(); break;
        case navigation::View::Menu: renderHome(); break;
    }

    renderFooter();

    if (state_.view() == navigation::View::Menu) {
        renderApplicationMenu();
    }
}

void App::renderHeader() const noexcept {
    using namespace ui::theme;

    drawText(34, 56, TextPrimary, 1.28f, "PSBB");
    // The graphite stroke recalls the legacy N; the red V is the Vita identity.
    vita2d_draw_rectangle(128, 26, 2, 34, RGBA8(66, 72, 82, 255));
    drawText(135, 56, Accent, 1.28f, "V");

    drawText(195, 42, TextSecondary, 0.58f, "PLAYSTATION BROADBAND VITA");
    drawText(195, 63, TextMuted, 0.48f, "THE NEXT GENERATION OF PLAYSTATION NAVIGATION");

    drawText(744, 39, TextMuted, 0.48f, "LOCAL SESSION");
    drawText(744, 61, TextSecondary, 0.52f, "FOUNDATION BUILD");
}

void App::renderNavigation() const noexcept {
    using namespace ui::theme;

    drawText(32, 119, TextMuted, 0.48f, "PLANNED CHANNELS");

    for (std::size_t i = 0; i < navigation::channelCount(); ++i) {
        const navigation::ChannelDefinition* channel = navigation::channelAt(i);
        if (channel == nullptr) {
            continue;
        }

        const int y = 137 + static_cast<int>(i) * 56;
        const bool selected = channel->availability == navigation::ChannelAvailability::Implemented && i == state_.selectedChannel();

        if (selected) {
            vita2d_draw_rectangle(28, y, 202, 45, PanelSelected);
            vita2d_draw_rectangle(28, y, 3, 45, Accent);
            vita2d_draw_rectangle(230, y, 1, 45, Border);
        }

        drawText(
            48,
            y + 28,
            selected ? TextPrimary : TextSecondary,
            0.72f,
            channel->title
        );
        if (channel->availability == navigation::ChannelAvailability::Planned)
            drawText(166, y + 27, TextMuted, 0.34f, "PLANNED");
        else
            drawText(208, y + 27, selected ? Accent : TextMuted, 0.55f, selected ? ">" : "");
    }
}

void App::renderHome() const noexcept {
    using namespace ui::theme;

    vita2d_draw_rectangle(270, 118, 654, 347, Panel);
    vita2d_draw_rectangle(270, 118, 654, 1, Border);
    vita2d_draw_rectangle(270, 118, 4, 347, AccentDim);

    drawText(305, 158, Accent, 0.52f, "WELCOME / 001");
    drawText(305, 211, TextPrimary, 1.34f, "YOUR PLAYSTATION WORLD");
    drawText(307, 244, TextSecondary, 0.68f, "A new home for the things you do on Vita.");

    drawText(307, 291, TextMuted, 0.56f, "CHANNEL DESTINATIONS ARE ROADMAP ENTRIES, NOT OPEN VIEWS.");
    drawText(307, 314, TextMuted, 0.56f, "NO CHANNEL IS SHIPPED IN THIS FOUNDATION BUILD.");

    constexpr int cardY = 354;
    constexpr int cardWidth = 184;
    constexpr int cardGap = 17;
    constexpr std::array<const char*, 3> cardTitles{{
        "GAMES",
        "APPLICATIONS",
        "SYSTEM"
    }};
    constexpr std::array<const char*, 3> cardDetails{{
        "PLANNED / NOT SHIPPED",
        "PLANNED CHANNEL",
        "PLANNED CHANNEL"
    }};

    for (std::size_t i = 0; i < cardTitles.size(); ++i) {
        const int x = 305 + static_cast<int>(i) * (cardWidth + cardGap);
        vita2d_draw_rectangle(x, cardY, cardWidth, 76, PanelRaised);
        vita2d_draw_rectangle(x, cardY, cardWidth, 2, i == 0 ? Accent : Border);
        drawText(x + 14, cardY + 31, TextPrimary, 0.62f, cardTitles[i]);
        drawText(x + 14, cardY + 54, TextMuted, 0.43f, cardDetails[i]);
    }

    drawText(305, 448, TextMuted, 0.48f, "ALL SIX CHANNELS ARE PLANNED / NONE ARE OPENABLE.");
}

void App::renderChannel() const noexcept {
    using namespace ui::theme;

    const navigation::ChannelDefinition* channel =
        navigation::channelAt(state_.selectedChannel());
    if (channel == nullptr || channel->availability != navigation::ChannelAvailability::Implemented) {
        renderHome();
        return;
    }

    // A channel is only openable after its implementation is integrated into
    // this Core build and registered as Implemented. Never imply readiness
    // merely because a development workspace exists in another repository.
    vita2d_draw_rectangle(270, 118, 654, 347, Panel);
    vita2d_draw_rectangle(270, 118, 654, 1, Border);
    vita2d_draw_rectangle(270, 118, 4, 347, Accent);
    drawText(305, 158, Accent, 0.52f, channel->title);
    drawText(305, 220, TextPrimary, 0.90f, "WORKSPACE NOT INTEGRATED");
    drawText(307, 258, TextSecondary, 0.58f, "This channel will become available in a future Core build.");
    drawText(307, 302, TextMuted, 0.48f, "Development repositories are not standalone Vita modules.");
    drawText(307, 486, TextMuted, 0.43f, "CIRCLE  BACK");
}

void App::renderAbout() const noexcept {
    using namespace ui::theme;

    vita2d_draw_rectangle(270, 118, 654, 347, Panel);
    vita2d_draw_rectangle(270, 118, 654, 1, Border);
    vita2d_draw_rectangle(270, 118, 4, 347, Accent);

    drawText(305, 158, Accent, 0.52f, "ABOUT THIS APPLICATION");
    drawText(305, 216, TextPrimary, 1.5f, "PSBBV");
    drawText(307, 251, TextSecondary, 0.68f, "PLAYSTATION BROADBAND VITA");
    drawText(307, 300, TextPrimary, 0.62f, "A native homebrew navigator for PlayStation Vita.");
    drawText(307, 330, TextSecondary, 0.56f, "Inspired by the channel-based spirit of PSBBN.");
    drawText(307, 389, TextMuted, 0.48f, "INDEPENDENT COMMUNITY PROJECT / NOT AFFILIATED WITH SONY");
    drawText(307, 438, TextMuted, 0.48f, "FOUNDATION BUILD / FEATURES ARE UNDER DEVELOPMENT");
}

void App::renderApplicationMenu() const noexcept {
    using namespace ui::theme;

    vita2d_draw_rectangle(0, 0, ScreenWidth, ScreenHeight, RGBA8(0, 0, 0, 170));
    vita2d_draw_rectangle(300, 176, 360, 224, Panel);
    vita2d_draw_rectangle(300, 176, 360, 2, Accent);
    drawText(330, 214, TextPrimary, 0.76f, "APPLICATION MENU");

    for (std::size_t i = 0; i < MenuItems.size(); ++i) {
        const int y = 238 + static_cast<int>(i) * 48;
        const bool selected = i == state_.selectedMenuItem();

        if (selected) {
            vita2d_draw_rectangle(320, y, 320, 40, PanelSelected);
            vita2d_draw_rectangle(320, y, 3, 40, Accent);
        }

        drawText(
            342,
            y + 26,
            selected ? TextPrimary : TextSecondary,
            0.62f,
            MenuItems[i]
        );
    }

    drawText(330, 383, TextMuted, 0.46f, "CIRCLE  CLOSE");
}

void App::renderFooter() const noexcept {
    using namespace ui::theme;

    vita2d_draw_rectangle(28, 492, 896, 1, Border);

    if (state_.view() == navigation::View::Menu) {
        drawText(32, 524, TextSecondary, 0.48f, "D-PAD  SELECT");
        drawText(196, 524, TextSecondary, 0.48f, "CROSS  CONFIRM");
    } else if (state_.view() == navigation::View::About ||
               state_.view() == navigation::View::Channel) {
        drawText(32, 524, TextSecondary, 0.48f, "CIRCLE  BACK");
    } else {
        drawText(32, 524, TextSecondary, 0.48f, "CHANNELS: PLANNED");
        drawText(196, 524, TextSecondary, 0.48f, "START: MENU");
    }

    drawText(744, 524, TextMuted, 0.48f, "START  MENU");
}

} // namespace psbbv
