/**
 * @file ITrackState.hpp
 * @brief Definește interfața abstractă pentru stările pistei (pattern State).
 */
#pragma once
#include "utils/lifesource.hpp"

#include "dto/DrivingCommandDTO.hpp"
#include "dto/SensorDataDTO.hpp"
#include "math/Vectors.hpp"

namespace ls {
class ATrackStateContext;

/**
 * @brief Interfața abstractă pentru o stare a pistei în pattern-ul State.
 *
 * Fiecare stare a pistei calculează comanda de conducere corespunzătoare
 * și decide tranziția la starea următoare pe baza datelor senzorilor.
 */
class ITrackState {
public:
  /// @brief Destructor pur virtual.
  virtual ~ITrackState() = 0;

  /**
   * @brief Calculează comanda de conducere pentru starea curentă.
   * @param sensorData Datele colectate de la senzorii mașinii.
   * @param ctx Contextul stărilor pistei (pentru tranziții).
   * @return Comanda de conducere calculată.
   */
  virtual const ls::DrivingCommandDTO
  computeCommand(const ls::SensorDataDTO &sensorData,
                 ATrackStateContext &ctx) = 0;

  /**
   * @brief Actualizează starea curentă cu următoarea stare potrivită.
   *
   * Verifică condițiile de tranziție și schimbă starea în context
   * dacă este necesar.
   *
   * @param sensorData Datele colectate de la senzorii mașinii.
   * @param ctx Contextul stărilor pistei (pentru setarea stării următoare).
   */
  virtual void updateNextState(const ls::SensorDataDTO &sensorData,
                               ATrackStateContext &ctx) const = 0;

protected:
};

} // namespace ls
