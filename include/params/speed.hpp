#pragma once

#include "utils/lifesource.hpp"
#include <cmath>

namespace ls {
class Speed {
public:
  static constexpr speed_t MAX = 100; // viteza maxima nelimitata
  static constexpr speed_t WAITING_CUBE_SPEED =
      30; // viteza maxima asteptand cubul
  static constexpr speed_t _4_WAY_CROSSWAY_SPEED =
      40; // viteza in intersectia cu 4 cai

public:
  static speed_t scale(const speed_t maxSpeed, const angle_t angle) {
    return (speed_t)((angle_t)maxSpeed * std::cos(angle));
  }

  static speed_t scale(const speed_t maxSpeed, const proximity_t cubeProxi) {
    return maxSpeed * (cubeProxi / 100);
  }
};
} // namespace ls
