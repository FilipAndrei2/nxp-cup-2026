#include "track_states/SeeingFinishLineSecondTimeState.hpp"
#include "params/Params.hpp"
#include "params/speed.hpp"
#include "track_states/ATrackStateContext.hpp"
#include "track_states/WaitingToAproachCubeState.hpp"
#include <array>

namespace ls {

const ls::DrivingCommandDTO SeeingFinishLineSecondTimeState::computeCommand(
    const ls::SensorDataDTO &sensorData, ATrackStateContext &ctx) {
  std::array<FVector2, 5> infoVectors;
  size_t numberInfoVectors = 0;
  for (auto &v : *sensorData.vectors) {
    if (numberInfoVectors >= 5) {
      break;
    }
    if (!Vectors::isFinishLine(v) && v != Vectors::ZERO) {
      infoVectors[numberInfoVectors] = v;
    }
  }

  angle_t angle;
  speed_t speed;
  switch (numberInfoVectors) {
  case 0: {
    // Fara vizibilitate -> ii dam orbeste inainte
    angle = 0.0f;
    speed = Speed::WAITING_CUBE_SPEED;
    break;
  }
  case 1: {
    // Vedem 1 vector -> calculam unghiul dupa el
    angle = Vector2<float>::AngleBetween(infoVectors[0], Vectors::NORTH);
    speed = Speed::scale(Speed::WAITING_CUBE_SPEED, angle);
    break;
  }

  case 2:
  default: {
    // Vedem 2 sau mai multi -> ii luam pe primi 2, calculam avg si unghi/speed
    FVector2 medi = Vector2<float>::Avg(infoVectors[0], infoVectors[1]);
    angle = Vector2<float>::AngleBetween(medi, Vectors::NORTH);
    speed = Speed::scale(Speed::WAITING_CUBE_SPEED, angle);
    break;
  }
  }

  // Urmeaza sa scalam viteza in functie de senzorul de proxi
  if (sensorData.cubeProximity > 0) {
    speed = speed * (sensorData.cubeProximity / 100);
  }

  return DrivingCommandDTO{
      .angle = angle, .speed = speed, .shouldStop = false}; // todo: finish
}

void SeeingFinishLineSecondTimeState::updateNextState(
    const ls::SensorDataDTO &sensorData, ATrackStateContext &ctx) const {
  // schimba stateul daca nu mai vezi fin, sau daca incepem sa percepem cubul
  if (Vectors::notSeeingFinishLine(*sensorData.vectors) ||
      sensorData.cubeProximity > Params::APPROACHING_CUBE_PERC_TSH) {
    ctx.setState(&WaitingToApproachCubeState::getInstance());
  }
}
} // namespace ls
