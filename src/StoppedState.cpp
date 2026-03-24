/**
 * @file StoppedState.cpp
 * @brief Implementarea stării finale de oprire a mașinii.
 */
#include "track_states/StoppedState.hpp"
#include "dto/SensorDataDTO.hpp"
#include "track_states/ATrackStateContext.hpp"

namespace ls {

/**
 * @brief Returnează comanda de oprire completă a mașinii.
 *
 * Setează unghiul la 0, viteza la 0 și @c shouldStop la @c true.
 *
 * @param sensorData Datele de la senzori (neutilizate în această stare).
 * @param ctx Contextul stărilor pistei (neutilizat).
 * @return Comanda de oprire completă.
 */
const DrivingCommandDTO
StoppedState::computeCommand(const SensorDataDTO &sensorData,
                             ATrackStateContext &ctx) {
  return DrivingCommandDTO{.angle = 0.0f, .speed = 0, .shouldStop = true};
}

/**
 * @brief Nu realizează nicio tranziție; mașina rămâne permanent oprită.
 *
 * @param sensorData Datele de la senzori (neutilizate).
 * @param ctx Contextul stărilor pistei (neutilizat).
 */
void StoppedState::updateNextState(const SensorDataDTO &sensorData,
                                   ATrackStateContext &ctx) const {
  /* Mașina nu mai iese din această stare. */
}

} // namespace ls
