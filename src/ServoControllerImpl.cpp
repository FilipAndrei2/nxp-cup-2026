/**
 * @file ServoControllerImpl.cpp
 * @brief Implementarea controllerului servo de direcție.
 */
#include "car/ServoControllerImpl.hpp"
#include "math/Angles.hpp"
#include "params/Params.hpp"
#include "servo.h"

namespace ls {

/**
 * @brief Inițializează servo-ul cu parametrii din @c Params.
 *
 * Apelează driverul hardware cu valorile minime, medii și maxime
 * ale ciclului de lucru PWM.
 */
ServoControllerImpl::ServoControllerImpl() {
  ServoInit(Params::SERVO_PWM_CHANNEL, Params::SERVO_MAX_DUTY_CYCLE,
            Params::SERVO_MIN_DUTY_CYCLE, Params::SERVO_MED_DUTY_CYCLE);
}

/**
 * @brief Direcționează roțile la unghiul specificat.
 *
 * Convertește unghiul din radiani într-o valoare de direcție întreagă
 * folosind @c Angles::angleToDir(), apoi trimite comanda la driver.
 *
 * @param angle Unghiul de direcție în radiani.
 */
void ServoControllerImpl::steer(ls::angle_t angle) {
  auto direction = Angles::angleToDir(angle);
  Steer(direction);
}

} // namespace ls
