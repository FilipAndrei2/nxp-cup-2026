/**
 * @file SpeedControllerImpl.cpp
 * @brief Implementarea controllerului de viteză (ESC).
 */
#include "car/SpeedControllerImpl.hpp"
#include "esc.h"
#include "params/Params.hpp"

namespace ls {

/**
 * @brief Inițializează ESC-ul cu parametrii din @c Params.
 *
 * Apelează driverul hardware cu canalul PWM și valorile minime, medii
 * și maxime ale ciclului de lucru.
 */
SpeedControllerImpl::SpeedControllerImpl() {
  EscInit(Params::ESC_PWM_CHANNEL, Params::ESC_MIN_DUTY_CYCLE,
          Params::ESC_MED_DUTY_CYCLE, Params::ESC_MAX_DUTY_CYCLE);
}

/**
 * @brief Modifică viteza mașinii prin trimiterea comenzii la driverul ESC.
 * @param speed Viteza dorită în intervalul [-100, 100].
 */
void SpeedControllerImpl::changeSpeed(ls::speed_t speed) { EscSetSpeed(speed); }

} // namespace ls
