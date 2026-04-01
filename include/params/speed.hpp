#pragma once

#include "utils/lifesource.hpp"
#include <cmath>

namespace ls {
class Speed {
public:
  static constexpr speed_t MAX = 20; // 10 -> Prea mic
  static constexpr speed_t _4_WAY_CROSSWAY_SPEED = 25; // viteza in intersectia cu 4 cai
  static constexpr speed_t WAITING_CUBE_SPEED = 2; // viteza maxima asteptand cubul
  static constexpr speed_t BREAK = -100;

public:
  static speed_t scale(const speed_t maxSpeed, const angle_t angle) {
    return (speed_t)((angle_t)maxSpeed * std::cos(angle));
  }

  static speed_t scale(const speed_t maxSpeed, const proximity_t cubeProxi) {
    return (speed_t)((float)maxSpeed * ((float)cubeProxi / 100.0f));
  }
};
} // namespace ls
