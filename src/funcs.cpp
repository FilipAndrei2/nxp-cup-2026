#include "algorithm/AlgorithmStrategyImpl.hpp"
#include "car/Car.hpp"
#include "car/CarBuilder.hpp"
#include "car/PixyCamControllerImpl.hpp"
#include "car/ServoControllerImpl.hpp"
#include "car/SpeedControllerImpl.hpp"
#include "car/UltrasoundSensorControllerImpl.hpp"

namespace ls {
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
}
