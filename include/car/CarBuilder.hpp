/**
 * @file CarBuilder.hpp
 * @brief Definește builder-ul pentru construirea obiectului @c Car.
 */
#pragma once
#include "utils/lifesource.hpp"

namespace ls {
class IPixyCamController;
class IServoController;
class ISpeedController;
class IUltrasoundSensorController;
class IAlgorithmStrategy;
class Car;

/**
 * @brief Builder pentru construirea unui obiect @c Car cu toate dependențele.
 *
 * Implementează pattern-ul Builder pentru a asigura că obiectul @c Car
 * este creat cu toate componentele necesare injectate. Aruncă excepție
 * dacă nu toate dependențele au fost setate înainte de apelul @c buildCar().
 */
class CarBuilder {
public:
  /// @brief Constructor implicit.
  CarBuilder() = default;

  /**
   * @brief Setează controllerul camerei Pixy2.
   * @param pcc Referință la implementarea controllerului camerei Pixy2.
   * @return Referință la acest builder (pentru înlănțuire de apeluri).
   */
  CarBuilder &setPixyCamController(IPixyCamController &pcc);

  /**
   * @brief Setează controllerul servo pentru direcție.
   * @param sc Referință la implementarea controllerului servo.
   * @return Referință la acest builder (pentru înlănțuire de apeluri).
   */
  CarBuilder &setServoController(IServoController &sc);

  /**
   * @brief Setează controllerul ESC pentru viteză.
   * @param sc Referință la implementarea controllerului de viteză.
   * @return Referință la acest builder (pentru înlănțuire de apeluri).
   */
  CarBuilder &setSpeedController(ISpeedController &sc);

  /**
   * @brief Setează controllerul senzorului ultrasonic.
   * @param us Referință la implementarea controllerului senzorului ultrasonic.
   * @return Referință la acest builder (pentru înlănțuire de apeluri).
   */
  CarBuilder &setUltrasoundSensorController(IUltrasoundSensorController &us);

  /**
   * @brief Setează strategia algoritmului de navigație.
   * @param algo Referință la implementarea algoritmului de navigație.
   * @return Referință la acest builder (pentru înlănțuire de apeluri).
   */
  CarBuilder &setAlgorithmStrat(IAlgorithmStrategy &algo);

  /**
   * @brief Construiește și returnează obiectul @c Car.
   * @throws std::runtime_error Dacă una sau mai multe dependențe nu au fost setate.
   * @return Un obiect @c Car complet configurat.
   */
  Car buildCar() const;

private:
  IPixyCamController *pixyCamController = nullptr;       ///< Pointer la controllerul camerei Pixy2.
  IServoController *servoController = nullptr;           ///< Pointer la controllerul servo.
  ISpeedController *speedController = nullptr;           ///< Pointer la controllerul ESC.
  IUltrasoundSensorController *ultrasoundController = nullptr; ///< Pointer la controllerul senzorului ultrasonic.
  IAlgorithmStrategy *algo = nullptr;                    ///< Pointer la algoritmul de navigație.
};

} // namespace ls
