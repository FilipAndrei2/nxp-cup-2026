#include "car/Car.hpp"
#include "main_functions.h"
#include "utils/lifesource.hpp"
#include <memory>

namespace ls {

int Car::run() {
  // MainLoop
  while (this->isRunning) {
    auto sensorData = this->readSensors();
    auto drivingCommand = this->computeDrivingCommand(sensorData);
    this->controlCar(drivingCommand);
  }

  return 0;
}

Car::Car(ls::Car::Context &ctx) : ctx(ctx) { this->isRunning = true; }

void Car::stopCar() { this->isRunning = false; }

SensorDataDTO Car::readSensors() {
  static std::shared_ptr<std::vector<FVector2>> vectors;
  vectors = this->ctx.pixySensor.getVectors();
  auto cubeProximity = this->ctx.ultrasoundSensor.cubeProximity();

  return {vectors, cubeProximity};
}

DrivingCommandDTO Car::computeDrivingCommand(const SensorDataDTO &sensorData) {
  return this->ctx.algorithm.computeParameters(sensorData);
}

void Car::controlCar(const DrivingCommandDTO drivingCommand) {
  if (drivingCommand.shouldStop) {
    this->stopCar();
  } else {
    this->ctx.servoController.steer(drivingCommand.angle);
    this->ctx.engineController.changeSpeed(drivingCommand.speed);
  }
}

} // namespace ls
