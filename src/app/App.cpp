#include "psbbv/app/App.hpp"

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdio>

#include <psp2/ctrl.h>

#include <vita2d.h>

#include "psbbv/navigation/ChannelRegistry.hpp"
#include "psbbv/navigation/MenuHitTest.hpp"
#include "psbbv/navigation/UiState.hpp"
#include "psbbv/nodes/NodeRegistry.hpp"
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
      touchWasDown_(false),
      gamesScan_() {
    gamesScan_.source = games::vita::makeVitaDiscoverySource(gamesScan_.providerContext);
    gamesScan_.job.id = 1;
}

App::~App() noexcept {
    shutdown();
}

void App::shutdown() noexcept {
    if (gamesScan_.threadId >= 0) {
        int exitStatus = 0;
        sceKernelWaitThreadEnd(gamesScan_.threadId, &exitStatus, nullptr);
        sceKernelDeleteThread(gamesScan_.threadId);
        gamesScan_.threadId = -1;
    }
}

void App::update(std::uint32_t buttons, int touchX, int touchY) noexcept {
    state_.update(buttons);
    const bool touchDown = touchX >= 0 && touchY >= 0;
    if (touchDown && !touchWasDown_) handleTouch(touchX, touchY);
    touchWasDown_ = touchDown;

    const std::uint32_t pressed = buttons & ~previousGamesButtons_;
    previousGamesButtons_ = buttons;
    if (state_.view() == navigation::View::Channel) {
        const navigation::ChannelDefinition* channel =
            navigation::channelAt(state_.selectedChannel());
        if (channel != nullptr && channel->id == navigation::ChannelId::Games) {
            if ((pressed & SCE_CTRL_CROSS) != 0 &&
                gamesScan_.lifecycle.status() == navigation::GamesDiscoveryStatus::Failed) {
                retryGamesDiscovery();
            }
            if (gamesScan_.lifecycle.status() == navigation::GamesDiscoveryStatus::Succeeded) {
                gamesBrowser_.setItemCount(gamesScan_.catalogue.size());
                if ((pressed & SCE_CTRL_UP) != 0) gamesBrowser_.moveUp();
                if ((pressed & SCE_CTRL_DOWN) != 0) gamesBrowser_.moveDown();
            }
        }
    }
    startGamesDiscovery();
}

bool App::shouldQuit() const noexcept { return state_.shouldQuit(); }

void App::startGamesDiscovery() noexcept {
    if (state_.view() != navigation::View::Channel) return;
    const navigation::ChannelDefinition* channel =
        navigation::channelAt(state_.selectedChannel());
    if (channel == nullptr || channel->id != navigation::ChannelId::Games) return;
    if (!gamesScan_.lifecycle.begin()) return;

    const SceUID thread = sceKernelCreateThread(
        "psbbv_games_scan", &App::gamesDiscoveryThread,
        0x10000100, 0x10000, 0, 0, nullptr);
    if (thread < 0) {
        startNodeJob(gamesScan_.job);
        failNodeJob(gamesScan_.job, thread, "Could not create the Games discovery worker");
        gamesScan_.lifecycle.finish(false);
        return;
    }

    gamesScan_.threadId = thread;
    const int started = sceKernelStartThread(thread, sizeof(App*), this);
    if (started < 0) {
        startNodeJob(gamesScan_.job);
        failNodeJob(gamesScan_.job, started, "Could not start the Games discovery worker");
        sceKernelDeleteThread(thread);
        gamesScan_.threadId = -1;
        gamesScan_.lifecycle.finish(false);
    }
}

void App::retryGamesDiscovery() noexcept {
    if (gamesScan_.lifecycle.status() != navigation::GamesDiscoveryStatus::Failed) return;

    // The worker publishes Failed immediately before returning. Join and
    // delete it before resetting any memory that the worker previously owned.
    if (gamesScan_.threadId >= 0) {
        int exitStatus = 0;
        if (sceKernelWaitThreadEnd(gamesScan_.threadId, &exitStatus, nullptr) < 0) return;
        if (sceKernelDeleteThread(gamesScan_.threadId) < 0) return;
        gamesScan_.threadId = -1;
    }

    if (!gamesScan_.lifecycle.retry()) return;
    gamesScan_.catalogue = games::Catalogue{};
    gamesScan_.report = games::DiscoveryReport{};
    gamesScan_.job = nodes::NodeJob{};
    gamesScan_.job.id = 1;
    gamesBrowser_.setItemCount(0);
}

int App::gamesDiscoveryThread(SceSize args, void* argument) noexcept {
    (void)args;
    auto* app = static_cast<App*>(argument);
    if (app == nullptr) return -1;

    GamesScanState& scan = app->gamesScan_;
    if (!startNodeJob(scan.job).ok()) {
        scan.lifecycle.finish(false);
        return -1;
    }

    scan.report = games::discoverInstalledTitles(
        scan.catalogue, scan.source, scan.scratch, sizeof(scan.scratch));
    if (scan.report.error == games::DiscoveryError::None) {
        completeNodeJob(scan.job);
        scan.lifecycle.finish(true);
    } else {
        failNodeJob(scan.job, static_cast<std::int32_t>(scan.report.error),
                    "Installed-title discovery failed");
        scan.lifecycle.finish(false);
    }
    return 0;
}

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
        const navigation::ChannelDefinition* channel =
            navigation::channelAt(state_.selectedChannel());
        if (channel != nullptr && channel->id == navigation::ChannelId::Games &&
            gamesScan_.lifecycle.status() == navigation::GamesDiscoveryStatus::Succeeded) {
            constexpr int firstRowTop = 302;
            constexpr int rowHeight = 27;
            if (x >= 305 && x <= 889 && y >= firstRowTop &&
                y < firstRowTop + rowHeight * static_cast<int>(navigation::GamesBrowserState::VisibleRows)) {
                const std::size_t row = static_cast<std::size_t>((y - firstRowTop) / rowHeight);
                gamesBrowser_.select(gamesBrowser_.firstVisibleIndex() + row);
            }
        }
        if (y >= 480 && x <= 180) state_.back();
        return;
    }
    constexpr int left=28, right=232, top=137, height=56;
    const std::size_t count=navigation::channelCount();
    if (x>=left && x<=right && y>=top && y<top+height*static_cast<int>(count)) {
        state_.openChannel(static_cast<std::size_t>((y-top)/height));
        return;
    }
    if (y>=354 && y<=430 && x>=305 && x<=889) {
        const int rel=x-305;
        if (rel<184) state_.openChannel(navigation::channelIndex(navigation::ChannelId::Games));
        else if (rel>=201 && rel<385) state_.openChannel(navigation::channelIndex(navigation::ChannelId::Applications));
        else if (rel>=402 && rel<586) state_.openChannel(navigation::channelIndex(navigation::ChannelId::System));
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

    drawText(32, 119, TextMuted, 0.48f, "NAVIGATION");

    for (std::size_t i = 0; i < navigation::channelCount(); ++i) {
        const navigation::ChannelDefinition* channel = navigation::channelAt(i);
        if (channel == nullptr) {
            continue;
        }

        const int y = 137 + static_cast<int>(i) * 56;
        const bool selected = i == state_.selectedChannel();

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

    const navigation::ChannelDefinition* channel =
        navigation::channelAt(state_.selectedChannel());
    if (channel == nullptr) {
        renderHome();
        return;
    }

    const nodes::NodeDefinition* node = nodes::findNode(channel->nodeId);
    const nodes::NodeAvailability status = node == nullptr
        ? nodes::NodeAvailability::Unavailable
        : nodes::effectiveAvailability(
            *node,
            nodes::HostApiMajor,
            nodes::HostApiMinor,
            nodes::NodePlatform::Vita
        );

    const char* availability = "UNAVAILABLE";
    switch (status) {
        case nodes::NodeAvailability::Ready: availability = "READY"; break;
        case nodes::NodeAvailability::Planned: availability = "PLANNED"; break;
        case nodes::NodeAvailability::Unavailable: availability = "UNAVAILABLE"; break;
        case nodes::NodeAvailability::Disabled: availability = "DISABLED"; break;
        case nodes::NodeAvailability::Error: availability = "ERROR"; break;
    }

    if (channel->id == navigation::ChannelId::Games) {
        renderGamesChannel(availability);
        return;
    }

    vita2d_draw_rectangle(270, 118, 654, 347, Panel);
    vita2d_draw_rectangle(270, 118, 654, 1, Border);
    vita2d_draw_rectangle(270, 118, 4, 347, Accent);

    drawText(305, 158, Accent, 0.52f, "CHANNEL /");
    drawText(392, 158, Accent, 0.52f, availability);
    drawText(305, 216, TextPrimary, 1.42f, channel->title);
    drawText(307, 252, TextSecondary, 0.72f, channel->tagline);

    vita2d_draw_rectangle(305, 286, 584, 112, PanelRaised);
    vita2d_draw_rectangle(305, 286, 3, 112, AccentDim);
    drawText(327, 321, TextPrimary, 0.66f, "CHANNEL STATUS");
    drawText(327, 351, TextSecondary, 0.56f, channel->description);

    if (channel->availability != navigation::ChannelAvailability::Implemented) {
        drawText(305, 438, TextMuted, 0.48f, "THIS CHANNEL IS NOT IMPLEMENTED IN THIS BUILD.");
    }
}

void App::renderGamesChannel(const char* availability) const noexcept {
    using namespace ui::theme;

    vita2d_draw_rectangle(270, 118, 654, 347, Panel);
    vita2d_draw_rectangle(270, 118, 654, 1, Border);
    vita2d_draw_rectangle(270, 118, 4, 347, Accent);

    drawText(305, 158, Accent, 0.52f, "GAMES /");
    drawText(392, 158, TextMuted, 0.52f, availability);
    drawText(305, 210, TextPrimary, 1.22f, "YOUR GAMES");
    drawText(307, 241, TextSecondary, 0.62f, "Read-only discovery from ux0:/app");

    const navigation::GamesDiscoveryStatus status = gamesScan_.lifecycle.status();
    char statusLine[96]{};
    if (status == navigation::GamesDiscoveryStatus::Idle) {
        std::snprintf(statusLine, sizeof(statusLine), "DISCOVERY NOT STARTED");
    } else if (status == navigation::GamesDiscoveryStatus::Running) {
        std::snprintf(statusLine, sizeof(statusLine), "SCANNING INSTALLED TITLES...");
    } else if (status == navigation::GamesDiscoveryStatus::Failed) {
        std::snprintf(statusLine, sizeof(statusLine), "SCAN FAILED / CODE %d",
                      static_cast<int>(gamesScan_.job.result.code));
    } else {
        std::snprintf(statusLine, sizeof(statusLine), "SCAN COMPLETE / %u TITLES",
                      static_cast<unsigned>(gamesScan_.catalogue.size()));
    }
    drawText(307, 280, TextSecondary, 0.56f, statusLine);

    if (status == navigation::GamesDiscoveryStatus::Running) {
        drawText(307, 330, TextMuted, 0.52f, "THE SCAN RUNS IN THE BACKGROUND.");
    } else if (status == navigation::GamesDiscoveryStatus::Failed) {
        drawText(307, 330, TextMuted, 0.50f, "CHECK ux0: THEN PRESS CROSS TO RETRY THE SCAN.");
    } else if (status == navigation::GamesDiscoveryStatus::Succeeded &&
               gamesScan_.catalogue.size() == 0) {
        drawText(307, 330, TextMuted, 0.52f, "NO VALID INSTALLED TITLES WERE FOUND.");
    } else if (status == navigation::GamesDiscoveryStatus::Succeeded) {
        const games::GameRecord* ordered[games::MaxGames]{};
        const std::size_t count = gamesScan_.catalogue.sorted(
            games::SortOrder::TitleAscending, ordered, games::MaxGames);
        const std::size_t first = gamesBrowser_.firstVisibleIndex();
        const std::size_t visible = std::min<std::size_t>(
            gamesBrowser_.visibleCount(), count > first ? count - first : 0);
        for (std::size_t i = 0; i < visible; ++i) {
            const std::size_t index = first + i;
            const int rowTop = 302 + static_cast<int>(i) * 27;
            const int y = rowTop + 17;
            const games::GameRecord* game = ordered[index];
            if (game == nullptr) continue;
            const bool selected = index == gamesBrowser_.selectedIndex();
            if (selected) vita2d_draw_rectangle(305, rowTop, 584, 24, PanelSelected);
            drawText(310, y, selected ? Accent : TextMuted, 0.43f, selected ? ">" : "");
            drawText(328, y, selected ? TextPrimary : TextSecondary, 0.54f, game->title);
            drawText(790, y, TextMuted, 0.43f, game->titleId);
            if (game->favorite) drawText(755, y, Accent, 0.43f, "*");
        }
        char browseLine[96]{};
        std::snprintf(browseLine, sizeof(browseLine),
                      "SHOWING %u-%u OF %u / UP-DOWN TO BROWSE",
                      static_cast<unsigned>(first + 1),
                      static_cast<unsigned>(first + visible),
                      static_cast<unsigned>(count));
        drawText(315, 463, TextMuted, 0.43f, browseLine);
    }

    drawText(305, 486, TextMuted, 0.43f, "DISCOVERY ONLY / GAME LAUNCH IS NOT ENABLED YET.");
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
    } else if (state_.view() == navigation::View::About) {
        drawText(32, 524, TextSecondary, 0.48f, "CIRCLE  BACK");
    } else if (state_.view() == navigation::View::Channel) {
        const navigation::ChannelDefinition* channel =
            navigation::channelAt(state_.selectedChannel());
        if (channel != nullptr && channel->id == navigation::ChannelId::Games) {
            if (gamesScan_.lifecycle.status() == navigation::GamesDiscoveryStatus::Failed)
                drawText(32, 524, TextSecondary, 0.48f, "CROSS  RETRY SCAN");
            else
                drawText(32, 524, TextSecondary, 0.48f, "D-PAD  BROWSE GAMES");
            drawText(236, 524, TextSecondary, 0.48f, "CIRCLE  BACK");
        } else {
            drawText(32, 524, TextSecondary, 0.48f, "CIRCLE  BACK");
        }
    } else {
        drawText(32, 524, TextSecondary, 0.48f, "D-PAD  NAVIGATE");
        drawText(196, 524, TextSecondary, 0.48f, "CROSS  OPEN CHANNEL");
    }

    drawText(744, 524, TextMuted, 0.48f, "START  MENU");
}

} // namespace psbbv
