#pragma once

#include "utils/lifesource.hpp"
#include <cmath>

namespace ls {
class Speed {
public:
  // MAX SPEEDS
  static constexpr speed_t MAX = 10; // 10 -> Prea mic
  static constexpr speed_t _4_WAY_CROSSWAY_SPEED = 10; // viteza in intersectia cu 4 cai
  static constexpr speed_t WAITING_CUBE_SPEED = 10; // viteza maxima asteptand cubul
  static constexpr speed_t BREAK = -100;
  static constexpr speed_t TEST_SPEED = 10;
  
  // THRESHOLD 
  // INTERVAL: [0, 100]
  static constexpr int STRAIGHT_ROAD  = 5; // Drum drept
  static constexpr int STEER_STEP_1   = 30;
  static constexpr int STEER_STEP_2   = 69;
  
  // SCALES
  // INTERVAL: (0, 100]
  // NOTE: 100 => masina nu are viteza
  //        1  => viteza maxima nu este scalata
  //
  // ATENTIE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  // XXX: NU SETA ORICARE DIN CONSTANTELE DE MAI JOS PE 0
  // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

  static constexpr speed_t ROAD_STEP_0_SCALE = 99;
  static constexpr speed_t ROAD_STEP_1_SCALE = 80;
  static constexpr speed_t ROAD_STEP_2_SCALE = 60;
  static constexpr speed_t ROAD_STEP_3_SCALE = 29;

  static_assert(ROAD_STEP_0_SCALE != 0 && ROAD_STEP_0_SCALE <= (speed_t)100, "EROARE: ROAD_STEP_0_SCALE E UN PROCENT IN INTERVALUL (0, 100]!");
  static_assert(ROAD_STEP_1_SCALE != 0 && ROAD_STEP_1_SCALE <= (speed_t)100, "EROARE: ROAD_STEP_1_SCALE E UN PROCENT IN INTERVALUL (0, 100]!");
  static_assert(ROAD_STEP_2_SCALE != 0 && ROAD_STEP_2_SCALE <= (speed_t)100, "EROARE: ROAD_STEP_2_SCALE E UN PROCENT IN INTERVALUL (0, 100]!");
  static_assert(ROAD_STEP_3_SCALE != 0 && ROAD_STEP_3_SCALE <= (speed_t)100, "EROARE: ROAD_STEP_3_SCALE E UN PROCENT IN INTERVALUL (0, 100]!");
  
  public:
  static constexpr speed_t Scale(const speed_t vmax, const speed_t perc) {
    return 100 * vmax / perc;
  }
};
} // namespace ls
