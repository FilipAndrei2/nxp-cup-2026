#pragma once
#include "cmath"
#include "numbers"
#include "utils/lifesource.hpp"

namespace ls {
class Angles {
public:
  static constexpr float FLOAT_PI =
      3.14159265358979323846264338327950288419716939937510f;

  static int angleToDir(angle_t angle) {
    // clamp
    if (angle < -FLOAT_PI / 2) angle = -FLOAT_PI / 2;
    if (angle >  FLOAT_PI / 2) angle =  FLOAT_PI / 2;

    // normalizare la [-1, 1]
    float normalized = angle / (FLOAT_PI / 2);

    // scalare la [-100, 100]
    return static_cast<int>(normalized * 100.0f);
}
};
} // namespace ls
