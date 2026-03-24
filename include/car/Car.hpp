/**
 * @file Car.hpp
 * @brief Definește clasa principală a mașinii de curse NXP Cup.
 */
#pragma once
#include "ICar.hpp"
#include "IPixyCamController.hpp"
#include "IServoController.hpp"
#include "ISpeedController.hpp"
#include "IUltrasoundSensorController.hpp"
#include "algorithm/IAlgorithmStrategy.hpp"
#include "utils/lifesource.hpp"

namespace ls {

/**
 * @brief Clasa principală care coordonează funcționarea mașinii de curse.
 *
 * Implementează @c ICar și conține bucla principală de control. Primește
 * prin injecție de dependențe toate componentele hardware (senzori și
 * actuatori) și algoritmul de navigație. Folosește pattern-ul Context
 * (@c Car::Context) pentru gruparea dependențelor.
 */
class Car : public ICar {
public:
  /**
   * @brief Structura de context ce grupează toate dependențele mașinii.
   */
  struct Context {
    IPixyCamController &pixySensor;           ///< Camera Pixy2 pentru detectarea liniei.
    IServoController &servoController;        ///< Controllerul servo pentru direcție.
    ISpeedController &engineController;       ///< Controllerul ESC pentru viteză.
    IUltrasoundSensorController &ultrasoundSensor; ///< Senzorul ultrasonic pentru detectarea cubului.
    IAlgorithmStrategy &algorithm;            ///< Algoritmul de navigație.
  };

public:
  /// @brief Constructorul implicit este șters (injecție de dependențe obligatorie).
  Car() = delete;

  /**
   * @brief Construiește mașina cu contextul de dependențe specificat.
   * @param ctx Referință la contextul ce conține toate componentele necesare.
   */
  Car(ls::Car::Context &ctx);

public:
  /**
   * @brief Rulează bucla principală de control a mașinii.
   * @return Cod de ieșire (0 la terminare normală).
   */
  virtual int run(void) override;

  /// @brief Destructor implicit.
  virtual ~Car() = default;

private:
  Car::Context &ctx;   ///< Referință la contextul de dependențe.
  bool isRunning;      ///< Indică dacă mașina este în funcțiune.

private:
  /// @brief Oprește mașina setând @c isRunning la @c false.
  void stopCar();

  /**
   * @brief Citește datele de la toți senzorii mașinii.
   * @return Un @c SensorDataDTO cu vectorii detectați și proximitatea cubului.
   */
  SensorDataDTO readSensors();

  /**
   * @brief Calculează comanda de conducere pe baza datelor senzorilor.
   * @param sensorData Datele colectate de la senzori.
   * @return Comanda de conducere calculată de algoritm.
   */
  DrivingCommandDTO computeDrivingCommand(const SensorDataDTO &sensorData);

  /**
   * @brief Aplică comanda de conducere pe actuatorii mașinii.
   * @param drivingCommand Comanda de conducere ce conține unghi, viteză și flag de oprire.
   */
  void controlCar(const DrivingCommandDTO drivingCommand);
};

} // namespace ls
