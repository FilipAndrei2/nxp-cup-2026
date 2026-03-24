/**
 * @file StartingBeforeFinishLineState.cpp
 * @brief Implementarea stării inițiale a mașinii (înainte de linia de finish).
 */
#include "track_states/StartingBeforeFinishLineState.hpp"
#include "math/Vectors.hpp"
#include "params/speed.hpp"
#include "track_states/ATrackStateContext.hpp"
#include "track_states/SeeingFinishFirstTimeState.hpp"

namespace ls {

/**
 * @brief Calculează comanda de conducere — mașina merge înainte cu viteză maximă.
 *
 * În starea inițială mașina avansează drept fără a analiza vectorii.
 *
 * @param sensorData Datele de la senzori (neutilizate în calcul).
 * @param ctx Contextul stărilor pistei.
 * @return Comanda cu unghi 0 și viteză maximă.
 */
const ls::DrivingCommandDTO StartingBeforeFinishLineState::computeCommand(
    const ls::SensorDataDTO &sensorData, ls::ATrackStateContext &ctx) {
  return DrivingCommandDTO{
      .angle = 0, .speed = Speed::MAX, .shouldStop = false};
}

/**
 * @brief Actualizează starea la @c SeeingFinishFirstState când este
 *        detectată linia de finish.
 * @param sensorData Datele de la senzori.
 * @param ctx Contextul stărilor pistei.
 */
void StartingBeforeFinishLineState::updateNextState(
    const ls::SensorDataDTO &sensorData, ATrackStateContext &ctx) const {
  if (Vectors::seeingFinishLine(*sensorData.vectors)) {
    ctx.setState(&SeeingFinishFirstState::getInstance());
  }
}

} // namespace ls
