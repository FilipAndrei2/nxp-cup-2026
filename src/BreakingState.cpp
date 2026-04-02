#include "track_states/BreakingState.hpp"
#include "track_states/ATrackStateContext.hpp"
#include "track_states/FinishedState.hpp"

namespace ls {
    void BreakingState::updateNextState(ATrackStateContext &ctx) const {
    if (this->cubeProximity > Params::SHOULD_END_BREAK) {
      ctx.setState(&FinishedState::getInstance());
    }
  }
}