/**
 * @file SeeingFinishLineSecondTimeState.cpp
 * @brief Implementarea stării în care mașina vede linia de finish a doua oară.
 */
#include "track_states/SeeingFinishLineSecondTimeState.hpp"
#include "params/Params.hpp"
#include "params/speed.hpp"
#include "track_states/ATrackStateContext.hpp"
#include "track_states/WaitingToAproachCubeState.hpp"
#include <array>

namespace ls {

/**
 * @brief Calculează comanda de conducere cu viteză redusă, adaptată pentru
 *        apropierea de cub după a doua trecere peste linia de finish.
 *
 * Algoritmul filtrează vectorii de linie (orizontali) și folosește vectorii
 * de direcție rămași pentru a calcula unghiul. Viteza este scalată ulterior
 * în funcție de proximitatea cubului dacă acesta este detectat.
 *
 * @param sensorData Datele de la senzori.
 * @param ctx Contextul stărilor pistei.
 * @return Comanda de conducere cu unghi și viteză adaptate.
 */
const ls::DrivingCommandDTO SeeingFinishLineSecondTimeState::computeCommand(
    const ls::SensorDataDTO &sensorData, ATrackStateContext &ctx) {
  std::array<FVector2, 5> infoVectors;
  size_t numberInfoVectors = 0;
  for (auto &v : *sensorData.vectors) {
    if (numberInfoVectors >= 5) {
      break;
    }
    if (!Vectors::isFinishLine(v) && v != Vectors::ZERO) {
      infoVectors[numberInfoVectors] = v;
    }
  }

  angle_t angle;
  speed_t speed;
  switch (numberInfoVectors) {
  case 0: {
    // Fara vizibilitate -> ii dam orbeste inainte
    angle = 0.0f;
    speed = Speed::WAITING_CUBE_SPEED;
    break;
  }
  case 1: {
    // Vedem 1 vector -> calculam unghiul dupa el
    angle = Vector2<float>::AngleBetween(infoVectors[0], Vectors::NORTH);
    speed = Speed::scale(Speed::WAITING_CUBE_SPEED, angle);
    break;
  }

  case 2:
  default: {
    // Vedem 2 sau mai multi -> ii luam pe primi 2, calculam avg si unghi/speed
    FVector2 medi = Vector2<float>::Avg(infoVectors[0], infoVectors[1]);
    angle = Vector2<float>::AngleBetween(medi, Vectors::NORTH);
    speed = Speed::scale(Speed::WAITING_CUBE_SPEED, angle);
    break;
  }
  }

  // Scalam viteza in functie de senzorul de proximitate al cubului
  if (sensorData.cubeProximity > 0) {
    speed = speed * (sensorData.cubeProximity / 100);
  }

  return DrivingCommandDTO{
      .angle = angle, .speed = speed, .shouldStop = false};
}

/**
 * @brief Actualizează starea la @c WaitingToApproachCubeState când linia
 *        de finish nu mai este vizibilă sau cubul este detectat.
 * @param sensorData Datele de la senzori.
 * @param ctx Contextul stărilor pistei.
 */
void SeeingFinishLineSecondTimeState::updateNextState(
    const ls::SensorDataDTO &sensorData, ATrackStateContext &ctx) const {
  if (Vectors::notSeeingFinishLine(*sensorData.vectors) ||
      sensorData.cubeProximity > Params::APPROACHING_CUBE_PERC_TSH) {
    ctx.setState(&WaitingToApproachCubeState::getInstance());
  }
}

} // namespace ls
