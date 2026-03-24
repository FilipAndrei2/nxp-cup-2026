/**
 * @file StartingBeforeFinishLineState.hpp
 * @brief Definește starea inițială a mașinii, înainte de prima trecere
 *        peste linia de finish.
 */
#pragma once
#include "dp/TSingleton.hpp"
#include "track_states/ITrackState.hpp"
#include "utils/lifesource.hpp"

namespace ls {

/**
 * @brief Starea inițială — mașina pornește înainte de a vedea linia de finish.
 *
 * Mașina circulă înainte cu viteză maximă fără a ține cont de vectori
 * specifici (este pe linia de start). Tranziția la starea
 * @c SeeingFinishFirstState se produce când este detectată linia de finish.
 *
 * Aceasta este starea inițială implicită a contextului @c ATrackStateContext.
 * Implementată ca Singleton pentru a asigura o singură instanță globală.
 */
class StartingBeforeFinishLineState
    : public ITrackState,
      public Singleton<StartingBeforeFinishLineState> {

  friend class Singleton<StartingBeforeFinishLineState>;

public:
  /// @brief Destructor implicit.
  ~StartingBeforeFinishLineState() = default;

  /**
   * @brief Calculează comanda de conducere (înainte cu viteză maximă).
   * @param sensorData Datele de la senzori.
   * @param ctx Contextul stărilor pistei.
   * @return Comanda de conducere cu unghi 0 și viteză maximă.
   */
  virtual const ls::DrivingCommandDTO
  computeCommand(const ls::SensorDataDTO &sensorData,
                 ATrackStateContext &ctx) override;

  /**
   * @brief Actualizează starea la @c SeeingFinishFirstState când este
   *        detectată linia de finish.
   * @param sensorData Datele de la senzori.
   * @param ctx Contextul stărilor pistei.
   */
  virtual void updateNextState(const ls::SensorDataDTO &,
                               ATrackStateContext &) const override;

private:
  /// @brief Constructor privat implicit (pattern Singleton).
  StartingBeforeFinishLineState() = default;
};

} // namespace ls
