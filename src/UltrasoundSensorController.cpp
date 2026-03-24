/**
 * @file UltrasoundSensorController.cpp
 * @brief Implementarea controllerului senzorului ultrasonic.
 */
#include "car/UltrasoundSensorControllerImpl.hpp"

namespace ls {

/**
 * @brief Returnează proximitatea cubului față de mașină.
 *
 * @note Implementare incompletă — returnează întotdeauna 0.
 *       Trebuie integrată cu driverul hardware al senzorului ultrasonic.
 *
 * @return Valoarea proximității (momentan returnează 0).
 */
uint8_t UltrasoundSensorController::cubeProximity() {
  return 0;
}

/**
 * @brief Inițializează controllerul senzorului ultrasonic.
 *
 * @note Implementare incompletă — inițializarea driverului urmează a fi realizată.
 */
UltrasoundSensorController::UltrasoundSensorController() {
  // todo: Initializare driver
}

} // namespace ls
