/**
 * @file SpeedControllerImpl.hpp
 * @brief Implementarea concretă a controllerului de viteză (ESC).
 */
#pragma once
#include "car/ISpeedController.hpp"
#include "dp/TSingleton.hpp"
#include "utils/lifesource.hpp"

namespace ls {

/**
 * @brief Implementarea concretă a controllerului de viteză.
 *
 * Inițializează ESC-ul și trimite comenzi de viteză prin intermediul
 * driverului hardware. Implementată ca Singleton pentru a asigura o
 * singură instanță globală.
 */
class SpeedControllerImpl : public ISpeedController,
                            public Singleton<SpeedControllerImpl> {
  friend class Singleton<SpeedControllerImpl>;

public:
  /**
   * @brief Modifică viteza mașinii.
   * @param speed Viteza dorită în intervalul [-100, 100].
   */
  virtual void changeSpeed(ls::speed_t speed) override;

  /// @brief Destructor virtual implicit.
  virtual ~SpeedControllerImpl() = default;

private:
  /// @brief Constructor privat; inițializează ESC-ul (pattern Singleton).
  SpeedControllerImpl();
};

} // namespace ls
