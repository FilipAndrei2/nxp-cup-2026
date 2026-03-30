#include "track_states/StartingState.hpp"
#include "dto/DrivingCommandDTO.hpp"
#include "math/Vectors.hpp"
#include "params/speed.hpp"
#include "track_states/ATrackStateContext.hpp"
#include "track_states/SeeingFirstFinishState.hpp"

namespace ls {

void StartingState::updateNextState(ATrackStateContext &ctx) const {
  // Schimbam stateul daca detectam linia de fin
  if (seeFinishLine) {
    ctx.setState(&SeeingFirstFinishState::getInstance());
  }
}

speed_t StartingState::computeSpeed(const angle_t inAngle,
                                    const proximity_t inCubeProxi) {
  return Speed::scale(Speed::MAX, inAngle);
}

} // namespace ls
