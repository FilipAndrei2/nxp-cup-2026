#include "track_states/BreakingState.hpp"
#include "params/speed.hpp"
#include "track_states/ATrackStateContext.hpp"
#include "track_states/FinishedState.hpp"

namespace ls {

void BreakingState::updateNextState(ATrackStateContext &ctx) const {
  if (this->cubeProximity > Params::SHOULD_END_BREAK) {
    ctx.setState(&FinishedState::getInstance());
  }
}

angle_t computeAngle() { return (angle_t)0; }

speed_t computeSpeed(const angle_t inAngle, const proximity_t inCubeProxi) {
  return Speed::BREAK;
}
} // namespace ls
