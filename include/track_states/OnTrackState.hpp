
/**
 * @file OnTrackState.hpp
 * @brief Definește starea de navigație normală pe pistă.
 */
#pragma once
#include "dp/TSingleton.hpp"
#include "dto/DrivingCommandDTO.hpp"
#include "track_states/ITrackState.hpp"
#include "utils/lifesource.hpp"

namespace ls {

/**
 * @brief Starea de navigație normală — mașina circulă pe pistă.
 *
 * Calculează unghiul de direcție și viteza pe baza vectorilor detectați
 * de camera Pixy2. Tranziția la starea @c SeeingFinishLineSecondTimeState
 * se produce când este detectată linia de finish.
 *
 * Implementată ca Singleton pentru a asigura o singură instanță globală.
 */
class OnTrackState : public Singleton<OnTrackState>, public ITrackState {
  friend class Singleton<OnTrackState>;

public:
  /**
   * @brief Calculează comanda de conducere în starea de navigație normală.
   * @param sensorData Datele de la senzori.
   * @param ctx Contextul stărilor pistei.
   * @return Comanda de conducere cu unghi și viteză calculate.
   */
  virtual const ls::DrivingCommandDTO
  computeCommand(const ls::SensorDataDTO &sensorData,
                 ATrackStateContext &ctx) override;

  /**
   * @brief Actualizează starea la @c SeeingFinishLineSecondTimeState
   *        când este detectată linia de finish.
   * @param sensorData Datele de la senzori.
   * @param ctx Contextul stărilor pistei.
   */
  virtual void updateNextState(const ls::SensorDataDTO &sensorData,
                               ATrackStateContext &ctx) const override;
};

} // namespace ls
