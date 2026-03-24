/**
 * @file IClock.hpp
 * @brief Definește interfața abstractă pentru un ceas de sistem.
 */
#pragma once

namespace ls {

/**
 * @brief Interfața abstractă pentru un ceas de sistem.
 *
 * Definește un contract de bază pentru orice implementare a unui ceas,
 * util pentru abstractizarea surselor de timp în testare și producție.
 */
class IClock {
public:
  /// @brief Destructor virtual implicit.
  virtual ~IClock() = default;
};

} // namespace ls
