#pragma once
#include "utils/lifesource.hpp"

#include <memory>

#include "math/Vector2.hpp"

namespace ls {

struct SensorDataDTO {
  const std::vector<ls::FVector2> &vectors;
  const ls::proximity_t cubeProximity;
};
} // namespace ls
