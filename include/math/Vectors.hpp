#pragma once
#include "math/Vector2.hpp"
#include <cmath>

namespace ls {
class Vectors {
public:
  static constexpr float HORIZONTAL_THRESHOLD = 0.1;

  template <class T> static bool isFinishLine(const Vector2<T> &vector) {
    return std::abs(vector.getY()) <= HORIZONTAL_THRESHOLD;
  }
};
} // namespace ls
