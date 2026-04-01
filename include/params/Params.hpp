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
  static constexpr size_t   ESC_INIT_DELAY      = 10'000'000U;
  static constexpr unsigned int ESC_PWM_CHANNEL = 0U;
  static constexpr uint16_t ESC_MIN_DUTY_CYCLE = 1638U;
  static constexpr uint16_t ESC_MED_DUTY_CYCLE = 2457U;
  static constexpr uint16_t ESC_MAX_DUTY_CYCLE = 3276U;

  // PIXY
  static constexpr unsigned int PIXY2_I2C_ADDRESS = 0x54U; // Setata in Pixymon
  static constexpr uint8_t PIXY2_I2C_CHANNEL = 0U;
  static constexpr uint32_t PIXY2CAM_FRAMERATE = 60;

  // SERVO
  static constexpr size_t   SERVO_INIT_DELAY = 10'000'000U;
  static constexpr unsigned int SERVO_PWM_CHANNEL = 1U;
  static constexpr uint16_t SERVO_MIN_DUTY_CYCLE = 1638U;
  static constexpr uint16_t SERVO_MED_DUTY_CYCLE = 2457U;
  static constexpr uint16_t SERVO_MAX_DUTY_CYCLE = 3276U;
};
} // namespace ls
