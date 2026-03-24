/**
 * @file ICar.hpp
 * @brief Definește interfața abstractă pentru mașina de curse NXP Cup.
 */
#pragma once

namespace ls {

/**
 * @brief Interfața de bază pentru mașina de curse.
 *
 * Orice implementare concretă a mașinii trebuie să moștenească această
 * interfață și să implementeze metoda @c run(), care conține bucla
 * principală de funcționare.
 */
class ICar {
public:
  /// @brief Destructor virtual implicit.
  virtual ~ICar() = default;

  /**
   * @brief Pornește bucla principală de funcționare a mașinii.
   * @return Cod de ieșire (0 la terminare normală).
   */
  virtual int run() = 0;
};

} // namespace ls