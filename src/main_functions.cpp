/**
 * @file main_functions.cpp
 * @brief Implementarea funcției de construire a mașinii cu toate dependențele.
 */
#include "algorithm/AlgorithmStrategyImpl.hpp"
#include "car/Car.hpp"
#include "car/CarBuilder.hpp"
#include "car/PixyCamControllerImpl.hpp"
#include "car/ServoControllerImpl.hpp"
#include "car/SpeedControllerImpl.hpp"
#include "car/UltrasoundSensorControllerImpl.hpp"

/**
 * @brief Construiește obiectul @c Car cu toate componentele singleton injectate.
 *
 * Utilizează @c CarBuilder pentru a asambla instanțele singleton ale tuturor
 * componentelor hardware și ale algoritmului de navigație.
 *
 * @return Un obiect @c Car complet configurat, gata de utilizare.
 */
ls::Car buildCar() {
  return ls::CarBuilder()
      .setPixyCamController(ls::PixyCamControllerImpl::getInstance())
      .setServoController(ls::ServoControllerImpl::getInstance())
      .setSpeedController(ls::SpeedControllerImpl::getInstance())
      .setUltrasoundSensorController(
          ls::UltrasoundSensorController::getInstance())
      .setAlgorithmStrat(ls::AlgorithmStrategyImpl::getInstance())
      .buildCar();
}
