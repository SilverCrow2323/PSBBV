#pragma once

#include <cstddef>
#include <cstdint>

namespace psbbv::nodes {

constexpr std::size_t NodeJobMessageCapacity = 96;

enum class NodeJobState : std::uint8_t { Queued, Running, Succeeded, Failed, Cancelled };
enum class NodeJobError : std::uint8_t { None, InvalidTransition, InvalidProgress, InvalidIdentity };
enum class NodeJobResultKind : std::uint8_t { None, Success, Cancelled, Error };

struct NodeJobResult final {
    NodeJobResultKind kind = NodeJobResultKind::None;
    std::int32_t code = 0;
    char message[NodeJobMessageCapacity]{};
};

struct NodeJob final {
    std::uint32_t id = 0;
    NodeJobState state = NodeJobState::Queued;
    std::uint8_t progress = 0;
    NodeJobResult result{};
};

struct NodeJobOperation final {
    NodeJobError error = NodeJobError::None;
    constexpr bool ok() const noexcept { return error == NodeJobError::None; }
};

// Pure, allocation-free job state policy. The host remains responsible for scheduling
// work and delivering these state changes on the UI thread.
NodeJobOperation startNodeJob(NodeJob& job) noexcept;
NodeJobOperation updateNodeJobProgress(NodeJob& job, std::uint8_t progress) noexcept;
NodeJobOperation completeNodeJob(NodeJob& job, std::int32_t resultCode = 0) noexcept;
NodeJobOperation failNodeJob(NodeJob& job, std::int32_t errorCode, const char* message) noexcept;
NodeJobOperation cancelNodeJob(NodeJob& job) noexcept;

} // namespace psbbv::nodes
