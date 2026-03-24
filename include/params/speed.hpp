/**
 * @file speed.hpp
 * @brief Definește clasa utilitară pentru calculul și scalarea vitezei.
 */
#pragma once

#include "utils/lifesource.hpp"
#include <cmath>

namespace ls {

/**
 * @brief Clasă utilitară pentru constante și calcule legate de viteza mașinii.
 *
 * Oferă constante pentru viteze predefinite și funcții de scalare a vitezei
 * în funcție de unghi sau de proximitatea cubului.
 */
class Speed {
public:
  static constexpr speed_t MAX = 100;              ///< Viteza maximă (fără limitare).
  static constexpr speed_t WAITING_CUBE_SPEED = 30; ///< Viteza maximă în modul de așteptare a cubului.
  static constexpr speed_t _4_WAY_CROSSWAY_SPEED = 40; ///< Viteza în intersecția cu 4 căi.

public:
  /**
   * @brief Scalează viteza în funcție de unghiul de direcție.
   *
   * Viteza este redusă proporțional cu cosinusul unghiului,
   * astfel încât viraje mai strânse implică o viteză mai mică.
   *
   * @param maxSpeed Viteza maximă de referință.
   * @param angle Unghiul de direcție curent (în radiani).
   * @return Viteza scalată corespunzător unghiului.
   */
  static speed_t scale(const speed_t maxSpeed, const angle_t angle) {
    return (speed_t)((angle_t)maxSpeed * std::cos(angle));
  }

  /**
   * @brief Scalează viteza în funcție de proximitatea cubului.
   *
   * Viteza este redusă proporțional cu procentul de proximitate al cubului.
   *
   * @param maxSpeed Viteza maximă de referință.
   * @param cubeProxi Proximitatea cubului (0–100%).
   * @return Viteza scalată în funcție de proximitate.
   */
  static speed_t scale(const speed_t maxSpeed, const proximity_t cubeProxi) {
    return maxSpeed * (cubeProxi / 100);
  }
};

} // namespace ls
