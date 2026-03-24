/**
 * @file UltrasoundSensorControllerImpl.hpp
 * @brief Implementarea concretă a controllerului senzorului ultrasonic.
 */
#pragma once
#include "car/IUltrasoundSensorController.hpp"
#include "dp/TSingleton.hpp"
#include "utils/lifesource.hpp"

namespace ls {

/**
 * @brief Implementarea concretă a controllerului senzorului ultrasonic.
 *
 * Gestionează comunicarea cu senzorul ultrasonic pentru detectarea
 * și măsurarea distanței față de cub. Implementată ca Singleton
 * pentru a asigura o singură instanță globală.
 */
class UltrasoundSensorController
    : public IUltrasoundSensorController,
      public Singleton<UltrasoundSensorController> {
  friend class Singleton<UltrasoundSensorController>;

public:
  /**
   * @brief Returnează proximitatea cubului față de mașină.
   * @return Valoarea proximității în intervalul [0, 100].
   */
  virtual uint8_t cubeProximity() override;

  /// @brief Destructor virtual implicit.
  virtual ~UltrasoundSensorController() = default;

private:
  /// @brief Constructor privat implicit (pattern Singleton).
  UltrasoundSensorController() = default;
};

} // namespace ls
