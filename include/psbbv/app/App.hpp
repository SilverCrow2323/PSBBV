#pragma once

#include <cstddef>
#include <cstdint>
#include <atomic>

#include <psp2/kernel/threadmgr.h>

#include <vita2d.h>

#include "psbbv/navigation/UiState.hpp"
#include "psbbv/navigation/GamesBrowserState.hpp"
#include "psbbv/navigation/GamesDiscoveryLifecycle.hpp"
#include "psbbv/games/Catalogue.hpp"
#include "psbbv/games/Discovery.hpp"
#include "psbbv/games/VitaDiscovery.hpp"
#include "psbbv/nodes/NodeJob.hpp"

namespace psbbv {

class App final {
public:
    explicit App(vita2d_pgf* font) noexcept;
    ~App() noexcept;
    void shutdown() noexcept;

    void update(
        std::uint32_t buttons,
        int touchX,
        int touchY
    ) noexcept;

    void render() const noexcept;

    [[nodiscard]] bool shouldQuit() const noexcept;

private:
    void handleTouch(int x, int y) noexcept;
    void startGamesDiscovery() noexcept;
    void retryGamesDiscovery() noexcept;
    void renderGamesChannel(const char* availability) const noexcept;
    static int gamesDiscoveryThread(SceSize args, void* argument) noexcept;

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
    navigation::GamesBrowserState gamesBrowser_;
    std::uint32_t previousGamesButtons_{0};

    struct GamesScanState final {
        games::Catalogue catalogue{};
        games::vita::VitaDiscoveryContext providerContext{};
        games::DiscoverySource source{};
        games::DiscoveryReport report{};
        std::uint8_t scratch[games::MaxParamSfoBytes]{};
        nodes::NodeJob job{};
        navigation::GamesDiscoveryLifecycle lifecycle{};
        SceUID threadId{-1};
    } gamesScan_;
};

} // namespace psbbv
