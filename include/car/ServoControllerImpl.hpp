/**
 * @file ServoControllerImpl.hpp
 * @brief Implementarea concretă a controllerului servo de direcție.
 */
#pragma once
#include "car/IServoController.hpp"
#include "utils/lifesource.hpp"
#include <dp/TSingleton.hpp>

namespace ls {

/**
 * @brief Implementarea concretă a controllerului servo.
 *
 * Inițializează servo-ul și trimite comenzi de direcție prin intermediul
 * driverului hardware. Implementată ca Singleton pentru a asigura o
 * singură instanță globală.
 */
class ServoControllerImpl : public IServoController,
                            public Singleton<ServoControllerImpl> {
  friend class Singleton<ServoControllerImpl>;

public:
  /// @brief Destructor virtual implicit.
  virtual ~ServoControllerImpl() = default;

  /**
   * @brief Direcționează roțile la unghiul specificat.
   * @param angle Unghiul de direcție exprimat în radiani; este convertit
   *              intern într-o valoare de direcție pentru driver.
   */
  virtual void steer(ls::angle_t angle) override;

private:
  /// @brief Constructor privat; inițializează servo-ul (pattern Singleton).
  ServoControllerImpl();
};

} // namespace ls
