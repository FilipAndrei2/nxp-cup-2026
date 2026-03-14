#pragma once
#include "pwm.h"
#include "utils/lifesource.hpp"
namespace ls {

class Params {
public:
  // VECTORS
  static constexpr uint8_t FINISH_LINE_NUMBER_OF_VECTORS_THSH = 1;

  // CUBE
  static constexpr proximity_t APPROACHING_CUBE_PERC_TSH = 20 /*%*/;
  static constexpr proximity_t STOP_CUBE_IS_TOO_CLOSE_TSH = 85 /*%*/;

  // ESC
  static constexpr Pwm_ChannelType ESC_PWM_CHANNEL = 0U;
  static constexpr uint16_t ESC_MIN_DUTY_CYCLE = 1638U;
  static constexpr uint16_t ESC_MED_DUTY_CYCLE = 2457U;
  static constexpr uint16_t ESC_MAX_DUTY_CYCLE = 3276U;

  // TODO:
  // Pixy
  static constexpr I2c_AddressType PIXY2_I2C_ADDRESS = 0U; // TODO:
  static constexpr uint8_t PIXY2_I2C_CHANNEL = 0U;

  // TODO:
  // SERVO
  static constexpr Pwm_ChannelType SERVO_PWM_CHANNEL = 0U;
  static constexpr uint16_t SERVO_MIN_DUTY_CYCLE = 0U;
  static constexpr uint16_t SERVO_MED_DUTY_CYCLE = 0U;
  static constexpr uint16_t SERVO_MAX_DUTY_CYCLE = 0U;

  // PIXY2
  static constexpr uint32_t PIXY2CAM_FRAMERATE =
      60; // todo: VERIFICA CORECTITUDINEA
};
} // namespace ls
