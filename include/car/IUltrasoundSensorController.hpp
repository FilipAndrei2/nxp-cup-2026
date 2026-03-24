/**
 * @file IUltrasoundSensorController.hpp
 * @brief Definește interfața abstractă pentru controllerul senzorului ultrasonic.
 */
#pragma once
#include "utils/lifesource.hpp"

namespace ls {

/**
 * @brief Interfața abstractă pentru controllerul senzorului ultrasonic.
 *
 * Definește contractul pentru citirea proximității cubului față de mașină,
 * prin intermediul unui senzor ultrasonic.
 */
class IUltrasoundSensorController {
public:
  /**
   * @brief Returnează proximitatea cubului față de mașină.
   * @return Valoarea proximității în intervalul [0, 100], unde:
   *         - 0 înseamnă că niciun cub nu este detectat,
   *         - 100 înseamnă că mașina trebuie oprită imediat (cub prea aproape).
   */
  virtual uint8_t cubeProximity() = 0;

  /// @brief Destructor virtual implicit.
  virtual ~IUltrasoundSensorController() = default;
};

} // namespace ls
