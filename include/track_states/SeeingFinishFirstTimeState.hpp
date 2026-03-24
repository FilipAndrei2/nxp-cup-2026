/**
 * @file SeeingFinishFirstTimeState.hpp
 * @brief Definește starea în care mașina vede linia de finish pentru prima dată.
 */
#pragma once
#include "dp/TSingleton.hpp"
#include "dto/DrivingCommandDTO.hpp"
#include "track_states/ITrackState.hpp"
#include "utils/lifesource.hpp"

namespace ls {

/**
 * @brief Starea în care mașina a detectat linia de finish pentru prima dată.
 *
 * Mașina continuă să avanseze drept cu viteză maximă. Tranziția la starea
 * @c OnTrackState se produce când linia de finish nu mai este vizibilă
 * (mașina a trecut peste ea).
 *
 * Implementată ca Singleton pentru a asigura o singură instanță globală.
 */
class SeeingFinishFirstState : public Singleton<SeeingFinishFirstState>,
                               public ITrackState {
  friend class Singleton<SeeingFinishFirstState>;

public:
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
   * @brief Actualizează starea la @c OnTrackState când linia de finish
   *        nu mai este vizibilă.
   * @param sensorData Datele de la senzori.
   * @param ctx Contextul stărilor pistei.
   */
  virtual void updateNextState(const ls::SensorDataDTO &sensorData,
                               ATrackStateContext &ctx) const override;
};

} // namespace ls
