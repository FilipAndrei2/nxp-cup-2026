#pragma once
#include <cmath>

namespace ls {
class Floats {
public:
  static constexpr float FLOAT_EPS = 1e-6; // Toleranta
  static inline bool eq(const float first, const float second) {
    return std::abs(first - second) < FLOAT_EPS;
  }
};
} // namespace ls
