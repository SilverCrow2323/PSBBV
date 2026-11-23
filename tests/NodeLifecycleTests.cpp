#include "psbbv/nodes/NodeLifecycle.hpp"

#include <cstdlib>
#include <iostream>

namespace {
void require(bool value, const char* message) {
    if (!value) { std::cerr << "FAIL: " << message << '\n'; std::exit(EXIT_FAILURE); }
}
}
int main() {
    using namespace psbbv::nodes;
    LifecycleState state = LifecycleState::Discovered;
    auto apply = [&state](LifecycleAction action) {
        const LifecycleResult result = transitionLifecycle(state, action);
        if (result.ok()) state = result.state;
        return result;
    };

    require(apply(LifecycleAction::Validate).state == LifecycleState::Validated, "discovered validates");
    require(apply(LifecycleAction::Prepare).state == LifecycleState::Prepared, "validated prepares");
    require(apply(LifecycleAction::Activate).state == LifecycleState::Active, "prepared activates");
    require(apply(LifecycleAction::Suspend).state == LifecycleState::Suspended, "active suspends");
    require(apply(LifecycleAction::Resume).state == LifecycleState::Active, "suspended resumes");
    require(apply(LifecycleAction::Deactivate).state == LifecycleState::Deactivated, "active deactivates");
    require(apply(LifecycleAction::Validate).error == LifecycleError::TerminalState,
            "deactivated node cannot restart");

    require(transitionLifecycle(LifecycleState::Discovered, LifecycleAction::Activate).error ==
                LifecycleError::InvalidTransition, "activation cannot skip validation and preparation");
    require(transitionLifecycle(LifecycleState::Failed, LifecycleAction::Resume).error ==
                LifecycleError::TerminalState, "failed node cannot resume");
    require(transitionLifecycle(LifecycleState::Prepared, LifecycleAction::Fail).state ==
                LifecycleState::Failed, "preparation failure is terminal");
    require(transitionLifecycle(LifecycleState::Suspended, LifecycleAction::Deactivate).state ==
                LifecycleState::Deactivated, "suspended node can deactivate");

    std::cout << "PSBBV node lifecycle policy: all checks passed.\n";
}
