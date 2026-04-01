#include "car/Car.hpp"
#include "car/PixyCamControllerImpl.hpp"
#include "main_functions.h"
#include "utils/lifesource.hpp"
#include <memory>

namespace ls {

int Car::run() {
  // MainLoop
	// PixyCamControllerImpl::changeLedColor(0, 255, 0);
	while (this->isRunning) {
		// PixyCamControllerImpl::changeLedColor(255, 0, 0);
    auto sensorData = this->readSensors();
    auto drivingCommand = this->computeDrivingCommand(sensorData);
    this->controlCar(drivingCommand);
    PixyCamControllerImpl::changeLedColor(0, 0, 0);
  }

  return 0;
}

Car::Car(ls::Car::Context ctx) : ctx(ctx) { this->isRunning = true; }

void Car::stopCar() { this->isRunning = false; }

SensorDataDTO Car::readSensors() {

  return {this->ctx.pixySensor.getVectors(),
          this->ctx.ultrasoundSensor.cubeProximity()};
}

DrivingCommandDTO Car::computeDrivingCommand(const SensorDataDTO &sensorData) {
  return this->ctx.algorithm.computeParameters(sensorData);
}

void Car::controlCar(const DrivingCommandDTO drivingCommand) {
  if (drivingCommand.shouldStop) {
    this->stopCar();
  } else {
    this->ctx.servoController.steer(drivingCommand.angle);
    auto speed = drivingCommand.speed;
    if (speed < 0) {
    	PixyCamControllerImpl::changeLedColor(255, 0, 0);
    } else if (speed >= 0&& speed < 50) {
    	PixyCamControllerImpl::changeLedColor(0, 255, 0);
    } else {
    	PixyCamControllerImpl::changeLedColor(0, 255, 255);
    }
    this->ctx.engineController.changeSpeed(drivingCommand.speed);
  }
}

} // namespace ls
