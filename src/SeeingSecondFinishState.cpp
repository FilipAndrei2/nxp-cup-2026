#include "track_states/SeeingSecondFinishState.hpp"
#include "params/Params.hpp"
#include "params/speed.hpp"
#include "track_states/ATrackStateContext.hpp"
#include "track_states/WaitingCubeState.hpp"
#include <array>

namespace ls {

void SeeingSecondFinishState::updateNextState(ATrackStateContext &ctx) const {
  // schimba stateul daca nu mai vezi fin, sau daca incepem sa percepem cubul
  if (!this->seeFinishLine) {
    ctx.setState(&WaitingCubeState::getInstance());
  }
}

speed_t
SeeingSecondFinishState::computeSpeed(const ls::angle_t angle,
                                      const ls::proximity_t cubeProximity) {
  return Speed::scale(Speed::WAITING_CUBE_SPEED, angle);
}
} // namespace ls
