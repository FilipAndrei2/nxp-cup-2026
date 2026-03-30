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
  auto angle = computeAngle();
  auto speed = computeSpeed(angle, sensorData.cubeProximity);
  return DrivingCommandDTO{.angle = angle, .speed = speed, .shouldStop = false};
}

void SeeingFirstFinishState::updateNextState(ATrackStateContext &ctx) const {
  if (!seeFinishLine) {
    ctx.setState(&OnTrackState::getInstance());
  }
}

speed_t SeeingFirstFinishState::computeSpeed(const angle_t inAngle,
                                             const proximity_t inCubeProxi) {
  return Speed::scale(Speed::WAITING_CUBE_SPEED, inAngle);
}
} // namespace  ls
