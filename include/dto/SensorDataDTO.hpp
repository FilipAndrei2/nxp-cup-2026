#pragma once
#include "utils/lifesource.hpp"

#include <memory>

#include "math/Vector2.hpp"

namespace ls {

struct SensorDataDTO {
  const std::vector<ls::FVector2> &vectors;

  /**
   * @brief Distanta pana la cub, in procente
   * cubeProximity = 0 inseamna cubul nu este detectat
   * cubeProximity = 100 inseamna cubul este detectat si trebuie ca masina sa se
   * opreasca.
   */
  const ls::proximity_t cubeProximity;
};
} // namespace ls
