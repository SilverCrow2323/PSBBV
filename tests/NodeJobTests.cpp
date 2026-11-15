#include "psbbv/nodes/NodeJob.hpp"

#include <cstdlib>
#include <cstring>
#include <iostream>

namespace {
void require(bool condition, const char* message) {
    if (!condition) { std::cerr << "FAIL: " << message << '\\n'; std::exit(EXIT_FAILURE); }
}
}

int main() {
    using namespace psbbv::nodes;
    NodeJob job{};
    require(!startNodeJob(job).ok() && job.state == NodeJobState::Queued,
            "zero job identity is rejected");
    job.id = 7;
    require(startNodeJob(job).ok() && job.state == NodeJobState::Running,
            "queued job starts exactly once");
    require(updateNodeJobProgress(job, 35).ok() && job.progress == 35,
            "running job reports progress");
    require(updateNodeJobProgress(job, 20).error == NodeJobError::InvalidProgress,
            "progress cannot move backwards");
    require(updateNodeJobProgress(job, 100).error == NodeJobError::InvalidProgress,
            "100 percent is reserved for successful completion");
    require(cancelNodeJob(job).ok() && job.state == NodeJobState::Cancelled,
            "running job can be cancelled");
    require(job.result.kind == NodeJobResultKind::Cancelled &&
            std::strcmp(job.result.message, "Cancelled") == 0,
            "cancellation has a typed terminal result");
    require(cancelNodeJob(job).error == NodeJobError::InvalidTransition,
            "terminal job cannot be cancelled twice");

    NodeJob success{};
    success.id = 8;
    require(startNodeJob(success).ok(), "second job starts");
    require(completeNodeJob(success, 42).ok(), "running job completes");
    require(success.state == NodeJobState::Succeeded && success.progress == 100 &&
            success.result.kind == NodeJobResultKind::Success && success.result.code == 42,
            "success result is typed and carries its code");
    require(updateNodeJobProgress(success, 50).error == NodeJobError::InvalidTransition,
            "terminal success cannot be mutated");

    NodeJob failure{};
    failure.id = 9;
    require(startNodeJob(failure).ok(), "failure job starts");
    require(failNodeJob(failure, -12, "catalog unavailable").ok(), "running job fails");
    require(failure.state == NodeJobState::Failed &&
            failure.result.kind == NodeJobResultKind::Error &&
            failure.result.code == -12 &&
            std::strcmp(failure.result.message, "catalog unavailable") == 0,
            "failure exposes bounded typed error details");

    NodeJob queued{};
    queued.id = 10;
    require(cancelNodeJob(queued).ok() && queued.state == NodeJobState::Cancelled,
            "queued job can be cancelled before execution");

    std::cout << "PSBBV node jobs: all checks passed.\\n";
    return EXIT_SUCCESS;
}
