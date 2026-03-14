#pragma once
#include "math/Vector2.hpp"
#include "pixy2.h"
#include "utils/lifesource.hpp"

namespace ls {
class IPixyCamController {
public:
  virtual ~IPixyCamController() = 0;
  virtual std::shared_ptr<std::vector<FVector2>> getVectors() = 0;
  static void changeLedColor(const int r, const int g, const int b) {
    Pixy2SetLed(r, g, b);
  }
};
} // namespace ls
