#pragma once

#include <atomic>
#include <cstdint>

namespace psbbv::navigation {

enum class GamesDiscoveryStatus : std::uint8_t {
    Idle,
    Running,
    Succeeded,
    Failed
};

// Atomic lifecycle for the asynchronous Games scan. A failed scan can return
// to Idle only through an explicit retry; successful scans stay one-shot.
class GamesDiscoveryLifecycle final {
public:
    bool begin() noexcept {
        GamesDiscoveryStatus expected = GamesDiscoveryStatus::Idle;
        return status_.compare_exchange_strong(
            expected, GamesDiscoveryStatus::Running,
            std::memory_order_acq_rel, std::memory_order_acquire);
    }

    bool finish(bool succeeded) noexcept {
        GamesDiscoveryStatus expected = GamesDiscoveryStatus::Running;
        return status_.compare_exchange_strong(
            expected,
            succeeded ? GamesDiscoveryStatus::Succeeded : GamesDiscoveryStatus::Failed,
            std::memory_order_release, std::memory_order_acquire);
    }

    bool retry() noexcept {
        GamesDiscoveryStatus expected = GamesDiscoveryStatus::Failed;
        return status_.compare_exchange_strong(
            expected, GamesDiscoveryStatus::Idle,
            std::memory_order_acq_rel, std::memory_order_acquire);
    }

    [[nodiscard]] GamesDiscoveryStatus status() const noexcept {
        return status_.load(std::memory_order_acquire);
    }

private:
    std::atomic<GamesDiscoveryStatus> status_{GamesDiscoveryStatus::Idle};
};

} // namespace psbbv::navigation
