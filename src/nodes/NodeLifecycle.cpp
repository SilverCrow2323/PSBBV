#include "psbbv/nodes/NodeLifecycle.hpp"

namespace psbbv::nodes {

LifecycleResult transitionLifecycle(LifecycleState current, LifecycleAction action) noexcept {
    if (current == LifecycleState::Deactivated) {
        return {current, LifecycleError::TerminalState};
    }

    if (action == LifecycleAction::Fail) {
        if (current == LifecycleState::Failed) return {current, LifecycleError::TerminalState};
        return {LifecycleState::Failed, LifecycleError::None};
    }
    if (action == LifecycleAction::Deactivate) {
        return {LifecycleState::Deactivated, LifecycleError::None};
    }

    switch (current) {
        case LifecycleState::Discovered:
            if (action == LifecycleAction::Validate) return {LifecycleState::Validated, LifecycleError::None};
            break;
        case LifecycleState::Validated:
            if (action == LifecycleAction::Prepare) return {LifecycleState::Prepared, LifecycleError::None};
            break;
        case LifecycleState::Prepared:
            if (action == LifecycleAction::Activate) return {LifecycleState::Active, LifecycleError::None};
            break;
        case LifecycleState::Active:
            if (action == LifecycleAction::Suspend) return {LifecycleState::Suspended, LifecycleError::None};
            break;
        case LifecycleState::Suspended:
            if (action == LifecycleAction::Resume) return {LifecycleState::Active, LifecycleError::None};
            break;
        case LifecycleState::Failed:
        case LifecycleState::Deactivated:
            return {current, LifecycleError::TerminalState};
    }
    return {current, LifecycleError::InvalidTransition};
}

} // namespace psbbv::nodes
