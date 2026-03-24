/**
 * @file Params.hpp
 * @brief Definește parametrii de configurare globali ai mașinii.
 */
#pragma once
#include "pwm.h"
#include "utils/lifesource.hpp"

namespace ls {

/**
 * @brief Clasă ce centralizează toți parametrii de configurare ai mașinii.
 *
 * Conține constante pentru praguri de detecție, configurații hardware
 * (ESC, servo, cameră Pixy2) și alți parametri globali ai sistemului.
 */
class Params {
public:
  /// @name Parametri vectori
  /// @{
  /// @brief Numărul minim de vectori orizontali pentru a detecta linia de finish.
  static constexpr uint8_t FINISH_LINE_NUMBER_OF_VECTORS_THSH = 1;
  /// @}

  /// @name Parametri detecție cub
  /// @{
  /// @brief Procentul de proximitate de la care mașina consideră că se apropie de cub.
  static constexpr proximity_t APPROACHING_CUBE_PERC_TSH = 20 /*%*/;
  /// @brief Procentul de proximitate la care mașina trebuie să se oprească (cubul este prea aproape).
  static constexpr proximity_t STOP_CUBE_IS_TOO_CLOSE_TSH = 85 /*%*/;
  /// @}

  /// @name Parametri ESC
  /// @{
  static constexpr Pwm_ChannelType ESC_PWM_CHANNEL = 0U;   ///< Canalul PWM al ESC-ului.
  static constexpr uint16_t ESC_MIN_DUTY_CYCLE = 1638U;    ///< Ciclu de lucru minim ESC.
  static constexpr uint16_t ESC_MED_DUTY_CYCLE = 2457U;    ///< Ciclu de lucru mediu (neutru) ESC.
  static constexpr uint16_t ESC_MAX_DUTY_CYCLE = 3276U;    ///< Ciclu de lucru maxim ESC.
  /// @}

  /// @name Parametri Pixy2
  /// @{
  static constexpr I2c_AddressType PIXY2_I2C_ADDRESS = 0U; ///< Adresa I2C a camerei Pixy2.
  static constexpr uint8_t PIXY2_I2C_CHANNEL = 0U;         ///< Canalul I2C utilizat de Pixy2.
  static constexpr uint32_t PIXY2CAM_FRAMERATE = 60;       ///< Rata de cadre a camerei Pixy2 (fps).
  /// @}

  /// @name Parametri servo
  /// @{
  static constexpr Pwm_ChannelType SERVO_PWM_CHANNEL = 0U; ///< Canalul PWM al servo-ului.
  static constexpr uint16_t SERVO_MIN_DUTY_CYCLE = 0U;     ///< Ciclu de lucru minim servo.
  static constexpr uint16_t SERVO_MED_DUTY_CYCLE = 0U;     ///< Ciclu de lucru mediu (centru) servo.
  static constexpr uint16_t SERVO_MAX_DUTY_CYCLE = 0U;     ///< Ciclu de lucru maxim servo.
  /// @}
};

} // namespace ls
