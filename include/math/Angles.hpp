#pragma once
#include "cmath"
#include "numbers"
#include "utils/lifesource.hpp"

namespace ls {
class Angles {
public:
  static constexpr float FLOAT_PI =
      3.14159265358979323846264338327950288419716939937510;

  /**
   * @param angle, un unghi intre [-(PI/2), si PI/2]
   */
  static int angleToDir(angle_t angle) {
    if (angle < (-FLOAT_PI / 2)) {
      angle = -FLOAT_PI / 2;
    } else if (angle > (FLOAT_PI / 2)) {
      angle = FLOAT_PI / 2;
    }

    return static_cast<int>((200.0f / FLOAT_PI) * angle);
  }
};
} // namespace ls
