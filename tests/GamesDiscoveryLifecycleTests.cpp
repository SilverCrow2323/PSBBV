#include "psbbv/navigation/GamesDiscoveryLifecycle.hpp"

#include <cstdlib>
#include <iostream>

namespace {
void require(bool condition, const char* message) {
    if (!condition) {
        std::cerr << "FAIL: " << message << '\\n';
        std::exit(EXIT_FAILURE);
    }
}
}

int main() {
    using namespace psbbv::navigation;
    GamesDiscoveryLifecycle scan;

    require(scan.status() == GamesDiscoveryStatus::Idle, "new scan starts idle");
    require(scan.begin(), "idle scan can start");
    require(scan.status() == GamesDiscoveryStatus::Running, "start publishes running state");
    require(!scan.begin(), "running scan cannot start twice");
    require(!scan.retry(), "running scan cannot be retried");
    require(scan.finish(true), "running scan can succeed");
    require(scan.status() == GamesDiscoveryStatus::Succeeded, "success is terminal");
    require(!scan.finish(false), "terminal scan cannot finish twice");
    require(!scan.retry(), "successful scan cannot be restarted accidentally");
    require(!scan.begin(), "successful scan remains one-shot");

    GamesDiscoveryLifecycle failed;
    require(failed.begin(), "second scan starts");
    require(failed.finish(false), "worker failure becomes retryable");
    require(failed.status() == GamesDiscoveryStatus::Failed, "failure is visible to the UI");
    require(!failed.begin(), "failed scan cannot silently auto-restart");
    require(failed.retry(), "explicit retry returns failure to idle");
    require(failed.status() == GamesDiscoveryStatus::Idle, "retry resets lifecycle to idle");
    require(failed.begin(), "retry can start exactly one new worker");
    require(!failed.retry(), "retry is unavailable while the new worker runs");
    require(failed.finish(true), "retried scan can complete");
    require(failed.status() == GamesDiscoveryStatus::Succeeded, "retried success is terminal");

    std::cout << "PSBBV Games discovery lifecycle: all checks passed.\\n";
    return EXIT_SUCCESS;
}
