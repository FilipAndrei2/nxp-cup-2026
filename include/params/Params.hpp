#pragma once
#include "utils/lifesource.hpp"

namespace ls {

class Params {
public:
  // VECTORS
  static constexpr uint8_t FINISH_LINE_NUMBER_OF_VECTORS_THSH = 1;

  // CUBE
  static constexpr proximity_t APPROACHING_CUBE_PERC_TSH = 20 /*%*/;
  static constexpr proximity_t SHOULD_STOP_CAR_CUBE_IS_CLOSE = 60;
  static constexpr proximity_t SHOULD_END_BREAK = 69;

  // ESC
  static constexpr unsigned int ESC_PWM_CHANNEL = 0U;
  static constexpr uint16_t ESC_MIN_DUTY_CYCLE = 1638U;
  static constexpr uint16_t ESC_MED_DUTY_CYCLE = 2457U;
  static constexpr uint16_t ESC_MAX_DUTY_CYCLE = 3276U;
  /* Busy-wait iterations after sending neutral to arm the ESC.
   * A volatile loop is used intentionally (consistent with the rest of the
   * codebase; single-threaded init, no concurrent tasks to block).
   * Actual duration varies with CPU clock and compiler optimisation level.
   * At 80 MHz and -O3 the store-load dependency (~5 cycles/iter) gives:
   *   32 000 000 ≈ 2 s  |  48 000 000 ≈ 3 s  |  64 000 000 ≈ 4 s
   * Increase this value if the ESC still beeps on startup. */
  static constexpr uint32_t ESC_ARM_DELAY_ITERATIONS = 64000000U;
  static_assert(ESC_ARM_DELAY_ITERATIONS > 0U,
                "ESC arming delay must be non-zero");
  static_assert(ESC_MIN_DUTY_CYCLE < ESC_MED_DUTY_CYCLE,
                "ESC duty cycles must satisfy MIN < MED");
  static_assert(ESC_MED_DUTY_CYCLE < ESC_MAX_DUTY_CYCLE,
                "ESC duty cycles must satisfy MED < MAX");

  // PIXY
  static constexpr unsigned int PIXY2_I2C_ADDRESS = 0x54U; // Setata in Pixymon
  static constexpr uint8_t PIXY2_I2C_CHANNEL = 0U;
  static constexpr uint32_t PIXY2CAM_FRAMERATE = 60;

  // SERVO
  static constexpr unsigned int SERVO_PWM_CHANNEL = 1U;
  static constexpr uint16_t SERVO_MIN_DUTY_CYCLE = 1638U;
  static constexpr uint16_t SERVO_MED_DUTY_CYCLE = 2457U;
  static constexpr uint16_t SERVO_MAX_DUTY_CYCLE = 3276U;
};
} // namespace ls
