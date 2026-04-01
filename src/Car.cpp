#include "car/Car.hpp"
#include "car/PixyCamControllerImpl.hpp"
#include "main_functions.h"
#include "utils/lifesource.hpp"
#include <memory>

namespace ls {

static void setLedByVectorNumber(size_t number) {
	PixyCamControllerImpl::changeLedColor(0, 0, 0);
	if (number == 0) {
		PixyCamControllerImpl::changeLedColor(255, 0, 0);
	} else if (number == 1) {
		PixyCamControllerImpl::changeLedColor(0, 255, 0);
	} else if (number == 2) {
		PixyCamControllerImpl::changeLedColor(0, 0, 255);
	} else {
		PixyCamControllerImpl::changeLedColor(255, 255, 255);
	}
}

static void setLedBySpeed(speed_t speed) {
	PixyCamControllerImpl::changeLedColor(0, 0, 0);

	    if (speed < 0) {
	    	PixyCamControllerImpl::changeLedColor(255, 0, 0);
	    } else if (speed >= 0&& speed < 50) {
	    	PixyCamControllerImpl::changeLedColor(0, 255, 0);
	    } else {
	    	PixyCamControllerImpl::changeLedColor(0, 255, 255);
	    }
}

static void setLedByAngle(angle_t angle) {
	PixyCamControllerImpl::changeLedColor(0, 0, 0);

	if (((int)(angle * 100000)) == 0) {
		PixyCamControllerImpl::changeLedColor(255, 255, 255);
	} // else if ()
}

int Car::run() {
  // MainLoop
	// PixyCamControllerImpl::changeLedColor(0, 255, 0);
	while (this->isRunning) {
		// PixyCamControllerImpl::changeLedColor(255, 0, 0);
    auto sensorData = this->readSensors();
    setLedByVectorNumber(sensorData.vectors->size());
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

    this->ctx.engineController.changeSpeed(drivingCommand.speed);
  }
}

} // namespace ls
