/**
 * @file PixyCamControllerImpl.hpp
 * @brief Implementarea concretă a controllerului camerei Pixy2.
 */
#pragma once
#include "car/IPixyCamController.hpp"
#include "dp/TSingleton.hpp"

#include <memory>

namespace ls {

/**
 * @brief Implementarea concretă a controllerului camerei Pixy2.
 *
 * Inițializează camera Pixy2 prin I2C și furnizează vectorii de linie
 * detectați normalizați. Implementată ca Singleton pentru a asigura
 * o singură instanță globală.
 */
class PixyCamControllerImpl : public IPixyCamController,
                              public Singleton<PixyCamControllerImpl> {
  friend class Singleton<PixyCamControllerImpl>;

  /**
   * @brief Returnează vectorii de direcție detectați de camera Pixy2.
   *
   * Vectorii returnați sunt normalizați (vectori de direcție unitari).
   * @return Un pointer partajat la lista de vectori detectați,
   *         sau @c nullptr dacă nu este detectat niciun vector.
   */
  virtual std::shared_ptr<std::vector<Vector2<float>>> getVectors() override;

  /// @brief Destructor implicit.
  ~PixyCamControllerImpl() = default;

private:
  /// @brief Constructor privat; inițializează camera Pixy2 (pattern Singleton).
  PixyCamControllerImpl();
};

} // namespace ls
