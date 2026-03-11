#pragma once
#include "utils/lifesource.hpp"
#include "math/Vector2.hpp"

namespace ls {
class IPixyCamController {
public:
  virtual std::vector<FVector2> getVectors() = 0;
};
} // namespace ls
