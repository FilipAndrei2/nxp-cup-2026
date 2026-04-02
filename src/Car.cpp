#include "car/Car.hpp"
#include "car/PixyCamControllerImpl.hpp"
#include "main_functions.h"
#include "utils/lifesource.hpp"
#include <memory>
#include "params/Params.hpp"

namespace ls {

#ifdef LS_DEBUG
/**
 * Rosu:  0 vectori
 * Verde: 1 vector
 * Albastru: 2 vectori
 * Alb: 3 sau mai multi vectori
 */
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

/**
 * Alb: Viteza = 0
 * Rosu: Reverse (viteza < 0)
 * Verde: Viteza intre (0, 50]
 * Albastru: Viteza Mare (> 50)
 */
static void setLedBySpeed(speed_t speed) {
	PixyCamControllerImpl::changeLedColor(0, 0, 0);

		if (speed == 0) {
			PixyCamControllerImpl::changeLedColor(255, 255, 255);
		}
	    else if (speed < 0) {
	    	PixyCamControllerImpl::changeLedColor(255, 0, 0);
	    } else if (speed > 0 && speed < 50) {
	    	PixyCamControllerImpl::changeLedColor(0, 255, 0);
	    } else {
	    	PixyCamControllerImpl::changeLedColor(0, 0, 255);
	    }
}

/**
 * Alb - Unghi ~= 0
 * Rosu = Unghi < 0
 * Blue = Unghi > 0
 */
static void setLedByAngle(angle_t angle) {
	PixyCamControllerImpl::changeLedColor(0, 0, 0);

	if (((int)(angle * 100000)) == 0) {
		PixyCamControllerImpl::changeLedColor(255, 255, 255);
	} else if (angle < 0) {
		PixyCamControllerImpl::changeLedColor(255, 0, 0);
	} else {
		PixyCamControllerImpl::changeLedColor(0, 0, 255);
	}
}

/**
 * Rosu: steer stanga
 * Verde: fara viraj
 * Albastru: steer dreapta
 */
static void setLedBySteer(int steer) {
	PixyCamControllerImpl::changeLedColor(0, 0, 0);
	if (steer < 0) {
		PixyCamControllerImpl::changeLedColor(255, 0, 0);
	} else if (steer == 0) {
		PixyCamControllerImpl::changeLedColor(0, 255, 0);
	} else {
		PixyCamControllerImpl::changeLedColor(0, 0, 255);
	}
}
#endif // LS_DEBUG 

// Main Loop
int Car::run() {
	// PixyCamControllerImpl::changeLedColor(0, 255, 0);
	while (this->isRunning) {
#ifdef LS_DEBUG
		// PixyCamControllerImpl::changeLedColor(255, 0, 0);
#endif
		auto sensorData = this->readSensors();
	auto drivingCommand = this->computeDrivingCommand(sensorData);
#ifdef LS_DEBUG
	setLedBySteer(drivingCommand.steer);
#endif
	this->controlCar(drivingCommand);
    PixyCamControllerImpl::changeLedColor(0, 0, 0);
  }

  return 0;
}

Car::Car(ls::Car::Context ctx) : ctx(ctx) { this->isRunning = true; }

void Car::stopCar() { this->isRunning = false; }

SensorDataDTO Car::readSensors() {

  return { 
			.vectors 	   = this->ctx.pixySensor.getVectors(),
          	.cubeProximity = this->ctx.ultrasoundSensor.cubeProximity()
		};
}

DrivingCommandDTO Car::computeDrivingCommand(const SensorDataDTO &sensorData) {
  return this->ctx.algorithm.computeParameters(sensorData);
}

void Car::controlCar(const DrivingCommandDTO drivingCommand) {
  if (drivingCommand.shouldStop) {
    this->stopCar();
  } else {
    this->ctx.servoController.steer(drivingCommand.steer);
    this->ctx.engineController.changeSpeed(drivingCommand.speed);
  }
}

} // namespace ls
