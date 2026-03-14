#pragma once

// includes
#include <memory>
#include <stdexcept>

#include <vector>

#include <cstdint>

#include "algorithm/AlgorithmStrategyImpl.hpp"
#include "car/Car.hpp"
#include "car/PixyCamControllerImpl.hpp"
#include "car/ServoControllerImpl.hpp"
#include "car/SpeedControllerImpl.hpp"
#include "car/UltrasoundSensorControllerImpl.hpp"

#include "main_functions.h"

#define TODO() static_assert(true == false, "TODO: must impl method");

namespace ls {

// Tipuri de date
using angle_t = float;
using speed_t = int32_t;
using proximity_t = uint32_t;

Car::Context createCarContext() {
  DriversInit();
  return Car::Context{
      .algorithm = ls::AlgorithmStrategyImpl::getInstance(),
      .engineController = ls::SpeedControllerImpl::getInstance(),
      .servoController = ls::ServoControllerImpl::getInstance(),
      .pixySensor = ls::PixyCamControllerImpl::getInstance(),
      .ultrasoundSensor = ls::UltrasoundSensorController::getInstance()};
}
} // namespace ls
