/**
 * @file Floats.hpp
 * @brief Utilitare pentru compararea numerelor în virgulă mobilă.
 */
#pragma once
#include <cmath>

namespace ls {

/**
 * @brief Clasă utilitară pentru compararea numerelor în virgulă mobilă.
 *
 * Oferă o toleranță epsilon și o metodă de comparare pentru egalitate,
 * evitând problemele de precizie specifice tipului @c float.
 */
class Floats {
public:
  /// @brief Toleranța utilizată la compararea numerelor în virgulă mobilă.
  static constexpr float FLOAT_EPS = 1e-6;

  /**
   * @brief Verifică dacă două valori @c float sunt aproximativ egale.
   * @param first Primul număr în virgulă mobilă.
   * @param second Al doilea număr în virgulă mobilă.
   * @return @c true dacă diferența absolută dintre cele două valori este mai
   *         mică decât @c FLOAT_EPS, altfel @c false.
   */
  static inline bool eq(const float first, const float second) {
    return std::abs(first - second) < FLOAT_EPS;
  }
};

} // namespace ls
