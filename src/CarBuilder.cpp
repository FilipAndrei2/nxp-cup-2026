/**
 * @file CarBuilder.cpp
 * @brief Implementarea metodelor clasei @c CarBuilder.
 */
#include "car/CarBuilder.hpp"
#include "car/Car.hpp"

namespace ls {

/**
 * @brief Setează strategia algoritmului de navigație.
 * @param algo Referință la implementarea algoritmului.
 * @return Referință la acest builder.
 */
CarBuilder &CarBuilder::setAlgorithmStrat(IAlgorithmStrategy &algo) {
  this->algo = &algo;
  return *this;
}

/**
 * @brief Setează controllerul camerei Pixy2.
 * @param pcc Referință la implementarea controllerului camerei Pixy2.
 * @return Referință la acest builder.
 */
CarBuilder &CarBuilder::setPixyCamController(IPixyCamController &pcc) {
  this->pixyCamController = &pcc;
  return *this;
}

/**
 * @brief Setează controllerul servo de direcție.
 * @param sc Referință la implementarea controllerului servo.
 * @return Referință la acest builder.
 */
CarBuilder &CarBuilder::setServoController(IServoController &sc) {
  this->servoController = &sc;
  return *this;
}

/**
 * @brief Setează controllerul ESC de viteză.
 * @param sc Referință la implementarea controllerului de viteză.
 * @return Referință la acest builder.
 */
CarBuilder &CarBuilder::setSpeedController(ISpeedController &sc) {
  this->speedController = &sc;
  return *this;
}

/**
 * @brief Setează controllerul senzorului ultrasonic.
 * @param us Referință la implementarea controllerului senzorului ultrasonic.
 * @return Referință la acest builder.
 */
CarBuilder &
CarBuilder::setUltrasoundSensorController(IUltrasoundSensorController &us) {
  this->ultrasoundController = &us;
  return *this;
}

/**
 * @brief Construiește și returnează obiectul @c Car cu toate dependențele injectate.
 * @throws std::runtime_error Dacă una sau mai multe dependențe nu au fost setate.
 * @return Un obiect @c Car complet configurat.
 */
Car CarBuilder::buildCar() const {
  if (!this->algo || !this->pixyCamController || !this->servoController ||
      !this->speedController || !this->ultrasoundController) {
    throw new std::runtime_error("Can't build car.");
  }

  auto ctx = Car::Context{.pixySensor = *this->pixyCamController,
                          .servoController = *this->servoController,
                          .engineController = *this->speedController,
                          .ultrasoundSensor = *this->ultrasoundController,
                          .algorithm = *this->algo};
  return Car(ctx);
}

} // namespace ls
