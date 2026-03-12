#pragma once
#include "utils/lifesource.hpp"

namespace ls {

class Params {
public:
  // VECTORS
  static constexpr uint8_t FINISH_LINE_NUMBER_OF_VECTORS_THSH = 1;

  // CUBE
  static constexpr proximity_t APPROACHING_CUBE_PERC_TSH = 20 /*%*/;
  static constexpr proximity_t STOP_CUBE_IS_TOO_CLOSE_TSH = 85 /*%*/;
};
} // namespace ls
