#include "psbbv/nodes/NodeJob.hpp"

#include <cstddef>

namespace psbbv::nodes {
namespace {
bool terminal(NodeJobState state) noexcept {
    return state == NodeJobState::Succeeded || state == NodeJobState::Failed ||
           state == NodeJobState::Cancelled;
}
void copyMessage(char* destination, const char* source) noexcept {
    std::size_t i = 0;
    if (source != nullptr) {
        while (i + 1 < NodeJobMessageCapacity && source[i] != '\\0') {
            destination[i] = source[i];
            ++i;
        }
    }
    destination[i] = '\\0';
}
NodeJobOperation invalidTransition() noexcept {
    return {NodeJobError::InvalidTransition};
}
}

NodeJobOperation startNodeJob(NodeJob& job) noexcept {
    if (job.id == 0) return {NodeJobError::InvalidIdentity};
    if (job.state != NodeJobState::Queued) return invalidTransition();
    job.state = NodeJobState::Running;
    return {};
}

NodeJobOperation updateNodeJobProgress(NodeJob& job, std::uint8_t progress) noexcept {
    if (job.state != NodeJobState::Running) return invalidTransition();
    if (progress > 99 || progress < job.progress) return {NodeJobError::InvalidProgress};
    job.progress = progress;
    return {};
}

NodeJobOperation completeNodeJob(NodeJob& job, std::int32_t resultCode) noexcept {
    if (job.state != NodeJobState::Running) return invalidTransition();
    job.state = NodeJobState::Succeeded;
    job.progress = 100;
    job.result.kind = NodeJobResultKind::Success;
    job.result.code = resultCode;
    return {};
}

NodeJobOperation failNodeJob(NodeJob& job, std::int32_t errorCode, const char* message) noexcept {
    if (job.state != NodeJobState::Running) return invalidTransition();
    job.state = NodeJobState::Failed;
    job.result.kind = NodeJobResultKind::Error;
    job.result.code = errorCode;
    copyMessage(job.result.message, message);
    return {};
}

NodeJobOperation cancelNodeJob(NodeJob& job) noexcept {
    if (terminal(job.state)) return invalidTransition();
    job.state = NodeJobState::Cancelled;
    job.result.kind = NodeJobResultKind::Cancelled;
    job.result.code = 0;
    copyMessage(job.result.message, "Cancelled");
    return {};
}

} // namespace psbbv::nodes
