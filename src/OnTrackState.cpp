/**
 * @file OnTrackState.cpp
 * @brief Implementarea stării de navigație normală pe pistă.
 */
#include "track_states/OnTrackState.hpp"
#include "params/Params.hpp"
#include "params/speed.hpp"
#include "track_states/ATrackStateContext.hpp"
#include "track_states/SeeingFinishLineSecondTimeState.hpp"
#include <array>

namespace ls {

/**
 * @brief Calculează comanda de conducere în starea de navigație normală.
 *
 * Algoritmul selectează până la 4 vectori relevanți (non-orizontali, nenuli)
 * și aplică una din strategiile de navigație în funcție de numărul lor:
 *  - 0 vectori: intersecție cu 4 căi — merge înainte cu viteză redusă.
 *  - 1 vector: calculează unghiul față de NORD și scalează viteza.
 *  - 2 vectori: calculează vectorul mediu, apoi unghiul și viteza.
 *  - 3+ vectori: folosește primii 2 vectori ca în cazul cu 2 vectori.
 *
 * @param sensorData Datele de la senzori.
 * @param ctx Contextul stărilor pistei.
 * @return Comanda de conducere cu unghi și viteză calculate.
 */
const ls::DrivingCommandDTO
OnTrackState::computeCommand(const ls::SensorDataDTO &sensorData,
                             ATrackStateContext &ctx) {
  std::array<FVector2, 4> infoVectors;
  uint8_t numberInfoVectors = 0;
  for (auto &v : *sensorData.vectors) {
    if (numberInfoVectors >= 4) {
      break;
    }
    if (!Vectors::isFinishLine(v) && v != Vectors::ZERO) {
      infoVectors[numberInfoVectors] = v;
      numberInfoVectors++;
    }
  }

  angle_t angle;
  speed_t speed;
  switch (numberInfoVectors) {
  case 0: {
    // Daca nu vedem niciun vector, probabil e in intersectie cu 4 cai, mergem
    // inainte
    angle = 0.0f;
    speed = Speed::_4_WAY_CROSSWAY_SPEED;
    break;
  }
  case 1: {
    // Pentru un vector, calculam unghiul intre el si vectorul nord, si scalam
    // viteza
    angle = Vector2<float>::AngleBetween(infoVectors[0], Vectors::NORTH);
    speed = Speed::scale(Speed::MAX, angle);
    break;
  }
  case 2: {
    // Pentru 2 vectori:
    // 1. se calculeaza un vector mediu M intre ei
    // 2. se calculeaza unghiul dintre vectorul mediu M si vectorul NORD
    // 3. se scaleaza viteza in functie de unghi
    FVector2 medi = Vector2<float>::Avg(infoVectors[0], infoVectors[1]);
    angle = Vector2<float>::AngleBetween(medi, Vectors::NORTH);
    speed = Speed::scale(Speed::MAX, angle);
    break;
  }
  default: {
    // Pentru 3 sau mai multi vectori:
    // Ii luam doar pe primi 2 si aplicam acelasi algoritm ca in cazul cu 2
    // vectori
    FVector2 medi = Vector2<float>::Avg(infoVectors[0], infoVectors[1]);
    angle = Vector2<float>::AngleBetween(infoVectors[0], infoVectors[1]);
    speed = Speed::scale(Speed::MAX, angle);
    break;
  }
  }
  return DrivingCommandDTO{.angle = angle, .speed = speed, .shouldStop = false};
}

/**
 * @brief Actualizează starea la @c SeeingFinishLineSecondTimeState
 *        când este detectată linia de finish.
 * @param sensorData Datele de la senzori.
 * @param ctx Contextul stărilor pistei.
 */
void OnTrackState::updateNextState(const ls::SensorDataDTO &sensorData,
                                   ATrackStateContext &ctx) const {
  if (Vectors::seeingFinishLine(*sensorData.vectors)) {
    ctx.setState(&SeeingFinishLineSecondTimeState::getInstance());
  }
}

} // namespace ls
