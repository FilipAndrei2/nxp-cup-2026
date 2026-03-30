#include "track_states/WaitingCubeState.hpp"
#include "math/Vector2.hpp"
#include "params/Params.hpp"
#include "params/speed.hpp"
#include "track_states/ATrackStateContext.hpp"
#include "track_states/BreakingState.hpp"

namespace ls {

speed_t WaitingCubeState::computeSpeed(const angle_t angle,
                                       const proximity_t cubeProximity) {
  return Speed::scale(Speed::WAITING_CUBE_SPEED, angle);
}

void WaitingCubeState::updateNextState(ATrackStateContext &ctx) const {
  // Schimbam starea pe STOP daca cubul este suficient de aproape.
  if (this->cubeDetected) {
    ctx.setState(&BreakingState::getInstance());
  }
}

} // namespace ls
