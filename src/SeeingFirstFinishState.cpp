#include "track_states/SeeingFirstFinishState.hpp"
#include "math/Vectors.hpp"
#include "params/speed.hpp"
#include "track_states/ATrackStateContext.hpp"
#include "track_states/OnTrackState.hpp"
#include "utils/lifesource.hpp"

namespace ls {
const ls::DrivingCommandDTO
SeeingFirstFinishState::computeCommand(const ls::SensorDataDTO &sensorData) {
  filterVectors(*sensorData.vectors);
  auto steer = computeSteer();
  auto speed = computeSpeed(steer, sensorData.cubeProximity);
  return DrivingCommandDTO{.steer = steer, .speed = speed, .shouldStop = false};
}

void SeeingFirstFinishState::updateNextState(ATrackStateContext &ctx) const {
  if (!seeFinishLine) {
    ctx.setState(&OnTrackState::getInstance());
  }
}

} // namespace  ls
