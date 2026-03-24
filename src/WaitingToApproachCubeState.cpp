/**
 * @file WaitingToApproachCubeState.cpp
 * @brief Implementarea stării de apropiere de cub.
 */
#include "params/Params.hpp"
#include "params/speed.hpp"
#include "track_states/ATrackStateContext.hpp"
#include "track_states/StoppedState.hpp"
#include "track_states/WaitingToAproachCubeState.hpp"

namespace ls {

/**
 * @brief Calculează comanda de conducere adaptată pentru apropierea de cub.
 *
 * Determină unghiul de direcție în funcție de vectorii detectați (0, 1 sau
 * mai mulți), calculează viteza de bază și o scalează în funcție de
 * proximitatea cubului.
 *
 * @param sensorData Datele de la senzori.
 * @param ctx Contextul stărilor pistei.
 * @return Comanda de conducere cu viteză redusă proporțional cu proximitatea cubului.
 */
const ls::DrivingCommandDTO
WaitingToApproachCubeState::computeCommand(const SensorDataDTO &sensorData,
                                           ATrackStateContext &ctx) {
  angle_t angle;
  speed_t speed;

  switch (sensorData.vectors->size()) {
  case 0: {
    angle = 0.0f;
    break;
  }

  case 1: {
    angle = Vector2<float>::AngleBetween(sensorData.vectors[0], Vectors::NORTH);
    break;
  }

  default: {
    FVector2 medi =
        Vector2<float>::Avg(sensorData.vectors[0], sensorData.vectors[1]);
    angle = Vector2<float>::AngleBetween(medi, Vectors::NORTH);
    break;
  }
  }
  speed = Speed::scale(Speed::WAITING_CUBE_SPEED, angle);
  speed = Speed::scale(speed, sensorData.cubeProximity);
  return DrivingCommandDTO{.angle = angle, .speed = speed, false};
}

/**
 * @brief Actualizează starea la @c StoppedState când cubul este suficient de aproape.
 *
 * Tranziția se produce când proximitatea cubului depășește pragul
 * @c Params::STOP_CUBE_IS_TOO_CLOSE_TSH.
 *
 * @param sensorData Datele de la senzori.
 * @param ctx Contextul stărilor pistei.
 */
void WaitingToApproachCubeState::updateNextState(
    const SensorDataDTO &sensorData, ATrackStateContext &ctx) const {
  if (sensorData.cubeProximity >= Params::STOP_CUBE_IS_TOO_CLOSE_TSH) {
    ctx.setState(&StoppedState::getInstance());
  }
}

} // namespace ls
