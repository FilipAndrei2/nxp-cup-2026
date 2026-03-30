#include "track_states/OnTrackState.hpp"
#include "params/Params.hpp"
#include "params/speed.hpp"
#include "track_states/ATrackStateContext.hpp"
#include "track_states/SeeingSecondFinishState.hpp"
#include "utils/lifesource.hpp"

namespace ls {
void OnTrackState::updateNextState(ATrackStateContext &ctx) const {
  // Schimbam stateul cand detectam linia de finish
  if (this->seeFinishLine) {
    ctx.setState(&SeeingSecondFinishState::getInstance());
  }
}

speed_t OnTrackState::computeSpeed(const angle_t inAngle,
                                   const proximity_t inCubeProxi) {
  return this->isInCrossway
             ? Speed::scale(Speed::_4_WAY_CROSSWAY_SPEED, inAngle)
             : Speed::scale(Speed::MAX, inAngle);
}

} // namespace ls
