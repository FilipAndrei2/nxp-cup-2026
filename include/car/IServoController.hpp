/**
 * @file IServoController.hpp
 * @brief Definește interfața abstractă pentru controllerul servo de direcție.
 */
#pragma once
#include "utils/lifesource.hpp"

namespace ls {

/**
 * @brief Interfața abstractă pentru controllerul servo de direcție.
 *
 * Definește contractul pentru controlul direcției mașinii prin intermediul
 * unui servo motor.
 */
class IServoController {
public:
  /**
   * @brief Direcționează roțile la unghiul specificat.
   * @param angle Unghiul de direcție exprimat în radiani.
   */
  virtual void steer(ls::angle_t angle) = 0;

  /// @brief Destructor pur virtual.
  virtual ~IServoController() = 0;
};

} // namespace ls
