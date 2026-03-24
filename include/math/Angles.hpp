/**
 * @file Angles.hpp
 * @brief Utilitare pentru conversia și manipularea unghiurilor.
 */
#pragma once
#include "cmath"
#include "numbers"
#include "utils/lifesource.hpp"

namespace ls {

/**
 * @brief Clasă utilitară pentru operații cu unghiuri.
 *
 * Oferă constante și funcții statice pentru conversia între reprezentări
 * ale unghiurilor utilizate în controlul direcției mașinii.
 */
class Angles {
public:
  /// @brief Valoarea aproximativă a lui PI cu precizie extinsă.
  static constexpr float FLOAT_PI =
      3.14159265358979323846264338327950288419716939937510;

  /**
   * @brief Convertește un unghi în radiani într-o valoare de direcție întreagă.
   *
   * Mapează intervalul [-(PI/2), PI/2] în intervalul [-100, 100],
   * trunchiind valorile în afara intervalului.
   *
   * @param angle Unghiul de convertit, exprimat în radiani, din intervalul
   *              [-(PI/2), PI/2].
   * @return Valoarea de direcție corespunzătoare (număr întreg).
   */
  static int angleToDir(angle_t angle) {
    if (angle < (-FLOAT_PI / 2)) {
      angle = -FLOAT_PI / 2;
    } else if (angle > (FLOAT_PI / 2)) {
      angle = FLOAT_PI / 2;
    }

    return static_cast<int>((200.0f / FLOAT_PI) * angle);
  }
};

} // namespace ls
