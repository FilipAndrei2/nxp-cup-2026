#pragma once
#include "utils/lifesource.hpp"

namespace ls {
class IPixyCamController;
class IServoController;
class ISpeedController;
class IUltrasoundSensorController;
class IAlgorithmStrategy;
class Car;

class CarBuilder {
public:
  CarBuilder() = default;
  CarBuilder &setPixyCamController(IPixyCamController &);
  CarBuilder &setServoController(IServoController &);
  CarBuilder &setSpeedController(ISpeedController &);
  CarBuilder &setUltrasoundSensorController(IUltrasoundSensorController &);
  CarBuilder &setAlgorithmStrat(IAlgorithmStrategy &);
  Car buildCar() const;

private:
  IPixyCamController *pixyCamController = nullptr;
  IServoController *servoController = nullptr;
  ISpeedController *speedController = nullptr;
  IUltrasoundSensorController *ultrasoundController = nullptr;
  IAlgorithmStrategy *algo = nullptr;
};
} // namespace ls
