#include "track_states/FinishedState.hpp"
#include "params/speed.hpp"
#include "track_states/ATrackStateContext.hpp"

namespace ls {

void FinishedState::updateNextState(ATrackStateContext &ctx) const {
  /* Nu mai iesim din stateul asta :) */
}

angle_t FinishedState::computeAngle() { return (angle_t)0; }

speed_t FinishedState::computeSpeed(angle_t angle, proximity_t cubeProximity) {
  return (speed_t)0;
}

bool FinishedState::shouldStopCar() { return true; }

} // namespace ls
