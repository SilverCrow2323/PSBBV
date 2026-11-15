#include "psbbv/app/App.hpp"

#include <array>
#include <cstddef>

#include <psp2/ctrl.h>

#include <vita2d.h>

#include "psbbv/ui/Theme.hpp"

namespace psbbv {
namespace {

constexpr int ScreenWidth = 960;
constexpr int ScreenHeight = 544;

struct ChannelInfo {
    const char* name;
    const char* tagline;
    const char* description;
};

constexpr std::array<ChannelInfo, 6> Channels{{
    {"GAMES", "Your games, gathered in one place.", "A future home for local game discovery and shortcuts."},
    {"APPLICATIONS", "Tools and homebrew, with room to grow.", "A future catalog for installed applications and utilities."},
    {"NETWORK", "A connected layer for digital experiences.", "Optional online experiences will be designed as separate modules."},
    {"MEDIA", "Your personal media, presented beautifully.", "A future library for locally available music, video, and images."},
    {"DOWNLOADS", "Keep track of content and transfers.", "A future workspace for transfer tasks and completed downloads."},
    {"SYSTEM", "Understand and configure your Vita.", "A future workspace for device information and PSBBV preferences."}
}};

constexpr std::array<const char*, 3> MenuItems{{
    "RESUME NAVIGATION",
    "ABOUT PSBBV",
    "EXIT APPLICATION"
}};

} // namespace

App::App(vita2d_pgf* font) noexcept
    : font_(font),
      previousButtons_(0),
      selectedChannel_(0),
      selectedMenuItem_(0),
      channelOpen_(false),
      applicationMenuOpen_(false),
      aboutOpen_(false),
      shouldQuit_(false) {
}

void App::update(
    std::uint32_t buttons,
    int touchX,
    int touchY
) noexcept {
    const std::uint32_t pressed = buttons & ~previousButtons_;
    previousButtons_ = buttons;

    if (touchX >= 0 && touchY >= 0) {
        handleTouch(touchX, touchY);
    }

    if (shouldQuit_) {
        return;
    }

    if ((pressed & SCE_CTRL_START) != 0) {
        applicationMenuOpen_ = !applicationMenuOpen_;
        aboutOpen_ = false;
        return;
    }

    if (applicationMenuOpen_) {
        if ((pressed & SCE_CTRL_UP) != 0) {
            selectedMenuItem_ =
                (selectedMenuItem_ + MenuItems.size() - 1) % MenuItems.size();
        }

        if ((pressed & SCE_CTRL_DOWN) != 0) {
            selectedMenuItem_ = (selectedMenuItem_ + 1) % MenuItems.size();
        }

        if ((pressed & SCE_CTRL_CIRCLE) != 0) {
            applicationMenuOpen_ = false;
        }

        if ((pressed & SCE_CTRL_CROSS) != 0) {
            activateMenuItem();
        }

        return;
    }

    if (aboutOpen_) {
        if ((pressed & (SCE_CTRL_CIRCLE | SCE_CTRL_CROSS)) != 0) {
            aboutOpen_ = false;
        }
        return;
    }

    if (channelOpen_) {
        if ((pressed & SCE_CTRL_CIRCLE) != 0) {
            channelOpen_ = false;
        }
        return;
    }

    if ((pressed & SCE_CTRL_UP) != 0) {
        selectedChannel_ =
            (selectedChannel_ + Channels.size() - 1) % Channels.size();
    }

    if ((pressed & SCE_CTRL_DOWN) != 0) {
        selectedChannel_ = (selectedChannel_ + 1) % Channels.size();
    }

    if ((pressed & SCE_CTRL_CROSS) != 0) {
        channelOpen_ = true;
    }
}

bool App::shouldQuit() const noexcept {
    return shouldQuit_;
}

void App::activateMenuItem() noexcept {
    switch (selectedMenuItem_) {
        case 0:
            applicationMenuOpen_ = false;
            break;
        case 1:
            applicationMenuOpen_ = false;
            aboutOpen_ = true;
            break;
        case 2:
            shouldQuit_ = true;
            break;
        default:
            selectedMenuItem_ = 0;
            applicationMenuOpen_ = false;
            break;
    }
}

void App::handleTouch(int x, int y) noexcept {
    if (applicationMenuOpen_) {
        constexpr int menuLeft = 320;
        constexpr int menuRight = 640;
        constexpr int firstItemTop = 238;
        constexpr int itemHeight = 48;

        if (x >= menuLeft && x <= menuRight &&
            y >= firstItemTop &&
            y < firstItemTop + itemHeight * static_cast<int>(MenuItems.size())) {
            selectedMenuItem_ =
                static_cast<std::size_t>((y - firstItemTop) / itemHeight);
            activateMenuItem();
        } else {
            applicationMenuOpen_ = false;
        }
        return;
    }

    if (aboutOpen_) {
        aboutOpen_ = false;
        return;
    }

    if (channelOpen_) {
        if (y >= 480 && x <= 180) {
            channelOpen_ = false;
        }
        return;
    }

    // Tapping a channel both focuses and opens it.
    constexpr int navigationLeft = 28;
    constexpr int navigationRight = 232;
    constexpr int firstRowTop = 137;
    constexpr int rowHeight = 56;
    if (x >= navigationLeft && x <= navigationRight &&
        y >= firstRowTop &&
        y < firstRowTop + rowHeight * static_cast<int>(Channels.size())) {
        selectedChannel_ =
            static_cast<std::size_t>((y - firstRowTop) / rowHeight);
        channelOpen_ = true;
        return;
    }

    // The three landing-page cards are direct shortcuts to their corresponding channels.
    if (y >= 354 && y <= 430 && x >= 305 && x <= 889) {
        const int relativeX = x - 305;
        if (relativeX < 184) {
            selectedChannel_ = 0;
        } else if (relativeX >= 201 && relativeX < 385) {
            selectedChannel_ = 1;
        } else if (relativeX >= 402 && relativeX < 586) {
            selectedChannel_ = 5;
        } else {
            return;
        }
        channelOpen_ = true;
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

    if (aboutOpen_) {
        renderAbout();
    } else if (channelOpen_) {
        renderChannel();
    } else {
        renderHome();
    }

    renderFooter();

    if (applicationMenuOpen_) {
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

    drawText(32, 119, TextMuted, 0.48f, "NAVIGATION");

    for (std::size_t i = 0; i < Channels.size(); ++i) {
        const int y = 137 + static_cast<int>(i) * 56;
        const bool selected = i == selectedChannel_;

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
            Channels[i].name
        );
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

    drawText(307, 291, TextMuted, 0.56f, "SELECT A CHANNEL TO ENTER ITS WORKSPACE.");
    drawText(307, 314, TextMuted, 0.56f, "BUILT AROUND THE SPIRIT OF PSBBN. DESIGNED FOR TODAY.");

    constexpr int cardY = 354;
    constexpr int cardWidth = 184;
    constexpr int cardGap = 17;
    constexpr std::array<const char*, 3> cardTitles{{
        "GAMES",
        "APPLICATIONS",
        "SYSTEM"
    }};
    constexpr std::array<const char*, 3> cardDetails{{
        "PLANNED CHANNEL",
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

    drawText(305, 448, TextMuted, 0.48f, "CHANNEL WORKSPACES ARE FOUNDATION PLACEHOLDERS.");
}

void App::renderChannel() const noexcept {
    using namespace ui::theme;

    const ChannelInfo& channel = Channels[selectedChannel_];

    vita2d_draw_rectangle(270, 118, 654, 347, Panel);
    vita2d_draw_rectangle(270, 118, 654, 1, Border);
    vita2d_draw_rectangle(270, 118, 4, 347, Accent);

    drawText(305, 158, Accent, 0.52f, "CHANNEL / PLANNED");
    drawText(305, 216, TextPrimary, 1.42f, channel.name);
    drawText(307, 252, TextSecondary, 0.72f, channel.tagline);

    vita2d_draw_rectangle(305, 286, 584, 112, PanelRaised);
    vita2d_draw_rectangle(305, 286, 3, 112, AccentDim);
    drawText(327, 321, TextPrimary, 0.66f, "FOUNDATION PLACEHOLDER");
    drawText(327, 351, TextSecondary, 0.56f, channel.description);

    drawText(305, 438, TextMuted, 0.48f, "THIS CHANNEL IS NOT IMPLEMENTED IN THIS BUILD.");
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
        const bool selected = i == selectedMenuItem_;

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

    if (applicationMenuOpen_) {
        drawText(32, 524, TextSecondary, 0.48f, "D-PAD  SELECT");
        drawText(196, 524, TextSecondary, 0.48f, "CROSS  CONFIRM");
    } else if (aboutOpen_) {
        drawText(32, 524, TextSecondary, 0.48f, "CIRCLE  BACK");
    } else if (channelOpen_) {
        drawText(32, 524, TextSecondary, 0.48f, "CIRCLE  BACK");
    } else {
        drawText(32, 524, TextSecondary, 0.48f, "D-PAD  NAVIGATE");
        drawText(196, 524, TextSecondary, 0.48f, "CROSS  OPEN CHANNEL");
    }

    drawText(744, 524, TextMuted, 0.48f, "START  MENU");
}

} // namespace psbbv
