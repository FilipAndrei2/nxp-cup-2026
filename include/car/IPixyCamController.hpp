#pragma once
#include "math/Vector2.hpp"
#include "utils/lifesource.hpp"

namespace ls {
class IPixyCamController {
public:
  virtual ~IPixyCamController() = 0;
  virtual std::shared_ptr<std::vector<FVector2>> getVectors() = 0;
};
} // namespace ls
