#include "car/CarBuilder.hpp"
#include "car/Car.hpp"

namespace ls {

CarBuilder &CarBuilder::setAlgorithmStrat(IAlgorithmStrategy &algo) {
  this->algo = &algo;
  return *this;
}

CarBuilder &CarBuilder::setPixyCamController(IPixyCamController &pcc) {
  this->pixyCamController = &pcc;
  return *this;
}

CarBuilder &CarBuilder::setServoController(IServoController &sc) {
  this->servoController = &sc;
  return *this;
}

CarBuilder &CarBuilder::setSpeedController(ISpeedController &sc) {
  this->speedController = &sc;
  return *this;
}

CarBuilder &
CarBuilder::setUltrasoundSensorController(IUltrasoundSensorController &us) {
  this->ultrasoundController = &us;
  return *this;
}

Car CarBuilder::buildCar() const {
  if (!this->algo || !this->pixyCamController || !this->servoController ||
      !this->speedController || !this->ultrasoundController) {
    throw std::runtime_error("Can't build car.");
  }

  auto ctx = Car::Context{.pixySensor = *this->pixyCamController,
                          .servoController = *this->servoController,
                          .engineController = *this->speedController,
                          .ultrasoundSensor = *this->ultrasoundController,
                          .algorithm = *this->algo};
  return Car(ctx);
}
} // namespace ls
