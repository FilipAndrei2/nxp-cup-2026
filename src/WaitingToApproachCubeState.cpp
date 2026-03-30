#include "math/Vector2.hpp"
#include "math/Vectors.hpp"
#include "params/Params.hpp"
#include "params/speed.hpp"
#include "track_states/ATrackStateContext.hpp"
#include "track_states/StoppedState.hpp"
#include "track_states/WaitingToAproachCubeState.hpp"
#include <array>

namespace ls {

const ls::DrivingCommandDTO
WaitingToApproachCubeState::computeCommand(const SensorDataDTO &sensorData) {
  angle_t angle;
  speed_t speed;

  // Filter out finish-line vectors and zero vectors, just like other states
  std::array<FVector2, 4> infoVectors;
  uint8_t numberInfoVectors = 0;
  for (auto &v : *sensorData.vectors) {
    if (numberInfoVectors >= infoVectors.size()) {
      break;
    }
    if (!Vectors::isFinishLine(v) && v != Vectors::ZERO) {
      infoVectors[numberInfoVectors] = v;
      numberInfoVectors++;
    }
  }

  switch (numberInfoVectors) {
  case 0: {
    angle = 0.0f;
    break;
  }

  case 1: {
    angle = Vector2<float>::AngleBetween(infoVectors[0], Vectors::NORTH);
    break;
  }

  default: {
    FVector2 medi = Vector2<float>::Avg(infoVectors[0], infoVectors[1]);
    angle = Vector2<float>::AngleBetween(medi, Vectors::NORTH);
    break;
  }
  }
  speed = Speed::scale(Speed::WAITING_CUBE_SPEED, angle);
  speed = Speed::scale(speed, sensorData.cubeProximity);
  return DrivingCommandDTO{.angle = angle, .speed = speed, .shouldStop = false};
}

void WaitingToApproachCubeState::updateNextState(
    const SensorDataDTO &sensorData, ATrackStateContext &ctx) const {
  // Schimbam starea pe STOP daca cubul este suficient de aproape.
  if (sensorData.cubeProximity >= Params::STOP_CUBE_IS_TOO_CLOSE_TSH) {
    ctx.setState(&StoppedState::getInstance());
  }
}

} // namespace ls
