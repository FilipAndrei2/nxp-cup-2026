/**
 * @file IPixyCamController.hpp
 * @brief Definește interfața abstractă pentru controllerul camerei Pixy2.
 */
#pragma once
#include "math/Vector2.hpp"
#include "pixy2.h"
#include "utils/lifesource.hpp"

namespace ls {

/**
 * @brief Interfața abstractă pentru controllerul camerei Pixy2.
 *
 * Definește contractul pentru citirea vectorilor de linie detectați de
 * camera Pixy2. Oferă și o metodă statică pentru schimbarea culorii LED-ului
 * camerei.
 */
class IPixyCamController {
public:
  /// @brief Destructor pur virtual.
  virtual ~IPixyCamController() = 0;

  /**
   * @brief Returnează vectorii de direcție detectați de camera Pixy2.
   *
   * Vectorii returnați sunt normalizați (vectori de direcție unitari).
   * @return Un pointer partajat la lista de vectori detectați,
   *         sau @c nullptr dacă nu este detectat niciun vector.
   */
  virtual std::shared_ptr<std::vector<Vector2<float>>> getVectors() = 0;

  /**
   * @brief Schimbă culoarea LED-ului camerei Pixy2.
   * @param r Componenta roșie (0–255).
   * @param g Componenta verde (0–255).
   * @param b Componenta albastră (0–255).
   */
  static void changeLedColor(const int r, const int g, const int b) {
    Pixy2SetLed(r, g, b);
  }
};

} // namespace ls
