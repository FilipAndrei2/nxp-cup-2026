/**
 * @file StoppedState.hpp
 * @brief Definește starea finală în care mașina este oprită complet.
 */
#pragma once
#include "dp/TSingleton.hpp"
#include "dto/DrivingCommandDTO.hpp"
#include "track_states/ITrackState.hpp"
#include "utils/lifesource.hpp"

namespace ls {

/**
 * @brief Starea finală — mașina este oprită și nu mai poate avansa.
 *
 * Odată intrată în această stare, mașina nu mai poate tranziționa în
 * altă stare. Comanda de conducere returnată va seta @c shouldStop = true,
 * unghiul la 0 și viteza la 0.
 *
 * Implementată ca Singleton pentru a asigura o singură instanță globală.
 */
class StoppedState : public Singleton<StoppedState>, public ITrackState {
  friend class Singleton<StoppedState>;

public:
  /**
   * @brief Returnează comanda de oprire completă a mașinii.
   * @param sensorData Datele de la senzori (neutilizate în această stare).
   * @param ctx Contextul stărilor pistei (neutilizat în această stare).
   * @return Comanda de conducere cu unghi 0, viteză 0 și @c shouldStop = true.
   */
  virtual const ls::DrivingCommandDTO
  computeCommand(const SensorDataDTO &sensorData,
                 ATrackStateContext &ctx) override;

  /**
   * @brief Nu realizează nicio tranziție; mașina rămâne oprită.
   * @param sensorData Datele de la senzori (neutilizate).
   * @param ctx Contextul stărilor pistei (neutilizat).
   */
  virtual void updateNextState(const SensorDataDTO &sensorData,
                               ATrackStateContext &ctx) const override;
};

} // namespace ls
