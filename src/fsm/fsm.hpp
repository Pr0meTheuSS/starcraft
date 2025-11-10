#pragma once
#include <utility>
#include <vector>

enum class State { Idle, PrepareToBuild };

using Transition = std::pair<State, State>;

namespace rts::fsm {
class FiniteStateMachine final {
   public:
    explicit FiniteStateMachine(State initState, const std::vector<Transition>& initTransitions) : mState(initState), mTransitions(initTransitions) {};
    State getState() const { return mState; };
    bool tryChangeState(State newState);
    const std::vector<Transition> getAvailableTransitions(State);

   private:
    State mState;
    std::vector<Transition> mTransitions;
};

}  // namespace rts::fsm
