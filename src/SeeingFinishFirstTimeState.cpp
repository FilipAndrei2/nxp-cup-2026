/**
 * @file SeeingFinishFirstTimeState.cpp
 * @brief Implementarea stării în care mașina vede linia de finish pentru prima dată.
 */
#include "track_states/SeeingFinishFirstTimeState.hpp"
#include "math/Vectors.hpp"
#include "params/speed.hpp"
#include "track_states/ATrackStateContext.hpp"
#include "track_states/OnTrackState.hpp"
#include "utils/lifesource.hpp"

namespace ls {

/**
 * @brief Calculează comanda de conducere — mașina merge înainte cu viteză maximă.
 *
 * În această stare mașina ignoră vectorii de direcție și avansează drept
 * pentru a traversa linia de finish.
 *
 * @param sensorData Datele de la senzori (neutilizate în calcul).
 * @param ctx Contextul stărilor pistei.
 * @return Comanda cu unghi 0 și viteză maximă.
 */
const ls::DrivingCommandDTO
SeeingFinishFirstState::computeCommand(const ls::SensorDataDTO &sensorData,
                                       ATrackStateContext &ctx) {
  return DrivingCommandDTO{
      .angle = 0.0f, .speed = Speed::MAX, .shouldStop = false};
}

/**
 * @brief Actualizează starea la @c OnTrackState când linia de finish
 *        nu mai este vizibilă (mașina a trecut peste ea).
 * @param sensorData Datele de la senzori.
 * @param ctx Contextul stărilor pistei.
 */
void SeeingFinishFirstState::updateNextState(
    const ls::SensorDataDTO &sensorData, ATrackStateContext &ctx) const {
  if (Vectors::notSeeingFinishLine(*sensorData.vectors)) {
    ctx.setState(&OnTrackState::getInstance());
  }
}

} // namespace  ls
