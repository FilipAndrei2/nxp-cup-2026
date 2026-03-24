/**
 * @file Car.cpp
 * @brief Implementarea metodelor clasei @c Car.
 */
#include "car/Car.hpp"
#include "main_functions.h"
#include "utils/lifesource.hpp"
#include <memory>

namespace ls {

/**
 * @brief Rulează bucla principală de control a mașinii.
 *
 * Citește datele de la senzori, calculează comanda de conducere și
 * aplică comanda pe actuatori, repetat până când @c isRunning devine @c false.
 *
 * @return 0 la terminarea normală a buclei.
 */
int Car::run() {
  while (this->isRunning) {
    auto sensorData = this->readSensors();
    auto drivingCommand = this->computeDrivingCommand(sensorData);
    this->controlCar(drivingCommand);
  }

  return 0;
}

/**
 * @brief Construiește mașina cu contextul de dependențe specificat.
 * @param ctx Referință la contextul ce conține toate componentele necesare.
 */
Car::Car(ls::Car::Context &ctx) : ctx(ctx) { this->isRunning = true; }

/**
 * @brief Oprește bucla principală setând indicatorul @c isRunning la @c false.
 */
void Car::stopCar() { this->isRunning = false; }

/**
 * @brief Citește datele de la toți senzorii mașinii.
 *
 * Obține vectorii de la camera Pixy2 și proximitatea cubului de la
 * senzorul ultrasonic.
 *
 * @return Un @c SensorDataDTO cu datele colectate.
 */
SensorDataDTO Car::readSensors() {
  static std::shared_ptr<std::vector<FVector2>> vectors;
  vectors = this->ctx.pixySensor.getVectors();
  auto cubeProximity = this->ctx.ultrasoundSensor.cubeProximity();

  return {vectors, cubeProximity};
}

/**
 * @brief Calculează comanda de conducere pe baza datelor senzorilor.
 * @param sensorData Datele colectate de la senzori.
 * @return Comanda de conducere calculată de algoritmul de navigație.
 */
DrivingCommandDTO Car::computeDrivingCommand(const SensorDataDTO &sensorData) {
  return this->ctx.algorithm.computeParameters(sensorData);
}

/**
 * @brief Aplică comanda de conducere pe actuatorii mașinii.
 *
 * Dacă @c shouldStop este @c true, oprește mașina. Altfel, trimite
 * unghiul de direcție la servo și viteza la ESC.
 *
 * @param drivingCommand Comanda de conducere de aplicat.
 */
void Car::controlCar(const DrivingCommandDTO drivingCommand) {
  if (drivingCommand.shouldStop) {
    this->stopCar();
  } else {
    this->ctx.servoController.steer(drivingCommand.angle);
    this->ctx.engineController.changeSpeed(drivingCommand.speed);
  }
}

} // namespace ls
