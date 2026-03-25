#include "track_states/SeeingFinishLineSecondTimeState.hpp"
#include "SeeingFinishLineSecondTimeImplFunctions.hpp"
#include "params/Params.hpp"
#include "params/speed.hpp"
#include "track_states/ATrackStateContext.hpp"
#include "track_states/WaitingToAproachCubeState.hpp"
#include <array>

namespace ls {

const ls::DrivingCommandDTO SeeingFinishLineSecondTimeState::computeCommand(
    const ls::SensorDataDTO &sensorData) {
  std::array<FVector2, 5> infoVectors;

  uint8_t numberInfoVectors =
      filterInfoVectors(*sensorData.vectors, infoVectors);

  angle_t angle;
  speed_t speed;

  computeSpeedAndAngle(infoVectors, numberInfoVectors, angle, speed);

  speed = scaleSpeedByCubeProximity(sensorData.cubeProximity, speed);

  return DrivingCommandDTO{
      .angle = angle, .speed = speed, .shouldStop = false}; // todo: finish
}

/**
 * @returns number of info vectors
 */
static uint8_t filterInfoVectors(const std::vector<FVector2> &inVectors,
                                 std::array<FVector2, 5> &outInfoVectors) {
  uint8_t numberInfoVectors = 0;
  for (auto &v : inVectors) {
    if (numberInfoVectors >= 5) {
      break;
    }
    if (!Vectors::isFinishLine(v) && v != Vectors::ZERO) {
      outInfoVectors[numberInfoVectors] = v;
      numberInfoVectors++;
    }
  }
  return numberInfoVectors;
}

static void computeSpeedAndAngle(const std::array<FVector2, 5> &inVectors,
                                 const uint8_t inNumberInfoVectors,
                                 angle_t &outAngle, speed_t &outSpeed) {
  switch (inNumberInfoVectors) {
  case 0: {
    // Fara vizibilitate -> ii dam orbeste inainte
    outAngle = 0.0f;
    outSpeed = Speed::WAITING_CUBE_SPEED;
    break;
  }
  case 1: {
    // Vedem 1 vector -> calculam unghiul dupa el
    outAngle = Vector2<float>::AngleBetween(inVectors[0], Vectors::NORTH);
    outSpeed = Speed::scale(Speed::WAITING_CUBE_SPEED, outAngle);
    break;
  }

  case 2:
  default: {
    // Vedem 2 sau mai multi -> ii luam pe primi 2, calculam avg si unghi/speed
    FVector2 medi = Vector2<float>::Avg(inVectors[0], inVectors[1]);
    outAngle = Vector2<float>::AngleBetween(medi, Vectors::NORTH);
    outSpeed = Speed::scale(Speed::WAITING_CUBE_SPEED, outAngle);
    break;
  }
  }
}

static speed_t scaleSpeedByCubeProximity(const proximity_t proximity,
                                         const speed_t maxSpeed) {
  if (proximity > 0) {
    return maxSpeed * (proximity / 100);
  }
  return maxSpeed;
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
