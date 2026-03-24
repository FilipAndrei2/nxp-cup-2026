/**
 * @file SensorDataDTO.hpp
 * @brief Definește structura de transfer a datelor de la senzori.
 */
#pragma once
#include "utils/lifesource.hpp"

#include <memory>

#include "math/Vector2.hpp"

namespace ls {

/**
 * @brief DTO (Data Transfer Object) pentru datele colectate de la senzori.
 *
 * Grupează datele primite de la camera Pixy2 și de la senzorul ultrasonic,
 * pentru a fi transmise algoritmului de navigație.
 */
struct SensorDataDTO {
  /// @brief Lista de vectori de linie detectați de camera Pixy2.
  const std::shared_ptr<std::vector<ls::FVector2>> &vectors;

  /**
   * @brief Proximitatea cubului față de mașină, exprimată procentual.
   *
   * Valoarea 0 înseamnă că niciun cub nu este detectat.
   * Valoarea 100 înseamnă că mașina trebuie oprită imediat (cubul este prea aproape).
   */
  const ls::proximity_t cubeProximity;
};

} // namespace ls
