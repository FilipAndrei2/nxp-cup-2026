/**
 * @file WaitingToAproachCubeState.hpp
 * @brief Definește starea de așteptare și aproach (approach) față de cub.
 */
#pragma once
#include "dp/TSingleton.hpp"
#include "dto/DrivingCommandDTO.hpp"
#include "track_states/ITrackState.hpp"
#include "utils/lifesource.hpp"

namespace ls {

/**
 * @brief Starea în care mașina se apropie lent de cub.
 *
 * Mașina circulă cu viteză redusă, scalată atât în funcție de unghi cât
 * și de proximitatea cubului. Tranziția la starea @c StoppedState se produce
 * când cubul este suficient de aproape (depășind pragul @c STOP_CUBE_IS_TOO_CLOSE_TSH).
 *
 * Implementată ca Singleton pentru a asigura o singură instanță globală.
 */
class WaitingToApproachCubeState : public Singleton<WaitingToApproachCubeState>,
                                   public ITrackState {
  friend class Singleton<WaitingToApproachCubeState>;

public:
  /**
   * @brief Calculează comanda de conducere adaptată pentru apropierea de cub.
   * @param sensorData Datele de la senzori.
   * @param ctx Contextul stărilor pistei.
   * @return Comanda de conducere cu viteză redusă în funcție de proximitatea cubului.
   */
  virtual const ls::DrivingCommandDTO
  computeCommand(const SensorDataDTO &sensorData,
                 ATrackStateContext &ctx) override;

  /**
   * @brief Actualizează starea la @c StoppedState când cubul este prea aproape.
   * @param sensorData Datele de la senzori.
   * @param ctx Contextul stărilor pistei.
   */
  virtual void updateNextState(const SensorDataDTO &sensorData,
                               ATrackStateContext &ctx) const override;
};

} // namespace ls
