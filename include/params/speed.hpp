#pragma once

#include "utils/lifesource.hpp"

namespace ls {
class Speed {
public:
  static constexpr speed_t MAX = 100; // viteza maxima nelimitata
  static constexpr speed_t WAITING_CUBE_SPEED =
      30; // viteza maxima asteptand cubul
  static constexpr speed_t _4_WAY_CROSSWAY_SPEED =
      40; // viteza in intersectia cu 4 cai
};
} // namespace ls
