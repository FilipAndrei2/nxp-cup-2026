

/**
 * @file SeeingFinishLineSecondTimeState.hpp
 * @brief Definește starea în care mașina vede linia de finish a doua oară.
 */
#pragma once
#include "dp/TSingleton.hpp"
#include "dto/DrivingCommandDTO.hpp"
#include "track_states/ITrackState.hpp"
#include "utils/lifesource.hpp"

namespace ls {

/**
 * @brief Starea în care mașina a detectat linia de finish a doua oară.
 *
 * Mașina circulă cu viteză redusă și urmărește vectorii de direcție,
 * adaptând viteza în funcție de proximitatea cubului. Tranziția la starea
 * @c WaitingToApproachCubeState se produce când linia de finish nu mai
 * este vizibilă sau când mașina se apropie de cub.
 *
 * Implementată ca Singleton pentru a asigura o singură instanță globală.
 */
class SeeingFinishLineSecondTimeState
    : public Singleton<SeeingFinishLineSecondTimeState>,
      public ITrackState {
  friend class Singleton<SeeingFinishLineSecondTimeState>;

public:
  /**
   * @brief Calculează comanda de conducere cu viteză redusă, ținând cont
   *        de proximitatea cubului.
   * @param sensorData Datele de la senzori.
   * @param ctx Contextul stărilor pistei.
   * @return Comanda de conducere adaptată pentru apropierea de cub.
   */
  virtual const ls::DrivingCommandDTO
  computeCommand(const ls::SensorDataDTO &sensorData,
                 ATrackStateContext &ctx) override;

  /**
   * @brief Actualizează starea la @c WaitingToApproachCubeState când
   *        linia de finish nu mai este vizibilă sau cubul este detectat.
   * @param sensorData Datele de la senzori.
   * @param ctx Contextul stărilor pistei.
   */
  virtual void updateNextState(const ls::SensorDataDTO &sensorData,
                               ATrackStateContext &ctx) const override;
};

} // namespace ls
