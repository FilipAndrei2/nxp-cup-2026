/**
 * @file AlgorithmStrategyImpl.hpp
 * @brief Implementarea concretă a strategiei algoritmului de navigație.
 */
#pragma once
#include "algorithm/IAlgorithmStrategy.hpp"
#include "dp/TSingleton.hpp"
#include "track_states/ATrackStateContext.hpp"
#include "track_states/ITrackState.hpp"
#include "utils/lifesource.hpp"

namespace ls {

/**
 * @brief Implementarea concretă a algoritmului de navigație.
 *
 * Această clasă implementează @c IAlgorithmStrategy folosind pattern-ul
 * State (prin @c ATrackStateContext) pentru a delega calculul comenzilor
 * de conducere stării curente a pistei. Este implementată ca Singleton
 * pentru a asigura o singură instanță globală.
 */
class AlgorithmStrategyImpl : public ATrackStateContext,
                              public IAlgorithmStrategy,
                              public Singleton<AlgorithmStrategyImpl> {
  friend class Singleton<AlgorithmStrategyImpl>;

public:
  /// @brief Destructor virtual implicit.
  virtual ~AlgorithmStrategyImpl() override = default;

  /**
   * @brief Calculează parametrii de conducere delegând stării curente a pistei.
   * @param sensorData Datele colectate de la senzorii mașinii.
   * @return Comanda de conducere calculată de starea curentă.
   */
  virtual const ls::DrivingCommandDTO
  computeParameters(const SensorDataDTO &sensorData) override;

private:
  /// @brief Constructor privat (pattern Singleton).
  AlgorithmStrategyImpl() = default;
};

} // namespace ls
