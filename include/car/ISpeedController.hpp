/**
 * @file ISpeedController.hpp
 * @brief Definește interfața abstractă pentru controllerul de viteză (ESC).
 */
#pragma once
#include "utils/lifesource.hpp"

namespace ls {

/**
 * @brief Interfața abstractă pentru controllerul de viteză al mașinii.
 *
 * Definește contractul pentru controlul vitezei mașinii prin intermediul
 * unui ESC (Electronic Speed Controller).
 */
class ISpeedController {
public:
  /**
   * @brief Modifică viteza mașinii.
   * @param speed Viteza dorită în intervalul [-100, 100].
   *              Valoarea -100 reprezintă viteza maximă în marșarier,
   *              iar 100 reprezintă viteza maximă înainte.
   */
  virtual void changeSpeed(ls::speed_t speed) = 0;

  /// @brief Destructor virtual implicit.
  virtual ~ISpeedController() = default;
};

} // namespace ls
