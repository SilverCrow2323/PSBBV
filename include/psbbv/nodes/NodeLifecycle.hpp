#pragma once

#include <cstdint>

namespace psbbv::nodes {

enum class LifecycleState : std::uint8_t {
    Discovered,
    Validated,
    Prepared,
    Active,
    Suspended,
    Failed,
    Deactivated
};

enum class LifecycleAction : std::uint8_t {
    Validate,
    Prepare,
    Activate,
    Suspend,
    Resume,
    Fail,
    Deactivate
};

enum class LifecycleError : std::uint8_t {
    None,
    InvalidTransition,
    TerminalState
};

struct LifecycleResult final {
    LifecycleState state;
    LifecycleError error;
    constexpr bool ok() const noexcept { return error == LifecycleError::None; }
};

// Pure transition policy. It does not call node code or perform resource operations.
LifecycleResult transitionLifecycle(
    LifecycleState current,
    LifecycleAction action
) noexcept;

} // namespace psbbv::nodes
