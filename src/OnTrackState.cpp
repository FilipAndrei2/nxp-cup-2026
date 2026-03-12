#include "track_states/OnTrackState.hpp"
#include "params/Params.hpp"
#include "params/speed.hpp"
#include "track_states/ATrackStateContext.hpp"
#include "track_states/SeeingFinishLineSecondTimeState.hpp"
#include <array>

namespace ls {

const ls::DrivingCommandDTO
OnTrackState::computeCommand(const ls::SensorDataDTO &sensorData,
                             ATrackStateContext &ctx) {
  std::array<FVector2, 4> infoVectors;
  uint8_t numberInfoVectors = 0;
  for (auto &v : sensorData.vectors) {
    if (numberInfoVectors >= 4) {
      break;
    }
    if (Vectors::isFinishLine(v) && v != Vectors::ZERO) {
      infoVectors[numberInfoVectors] = v;
      numberInfoVectors++;
    }
  }

  switch (numberInfoVectors) {
  case 0: {
    // Daca nu vedem niciun vector, probabil e in intersectie cu 4 cai, mergem
    // inainte
    return DrivingCommandDTO{.angle = 0.0f,
                             .speed = Speed::_4_WAY_CROSSWAY_SPEED,
                             .shouldStop = false};
  }
  case 1: {
    // Pentru un vector, calculam unghiul intre el si vectorul nord, si scalam
    // viteza

    break;
  }
  case 2: {
    // Pentru 2 vectori:
    // 1. se calculeaza un vector mediu M intre ei
    // 2. se calculeaza unghiul dintre vectorul mediu M si vectorul NORD
    // 3. se scaleaza viteza in functie de unghi
    break;
  }
  default: {
    // Pentru 3 sau mai multi vectori:
    // Ii luam doar pe primi 2 si aplicam acelasi algoritm ca in cazul cu 2
    // vectori
    break;
  }
  }
}

void OnTrackState::updateNextState(const ls::SensorDataDTO &sensorData,
                                   ATrackStateContext &ctx) const {
  // Schimbam stateul cand detectam linia de finish
  if (Vectors::seeingFinishLine(sensorData.vectors)) {
    ctx.setState(&SeeingFinishLineSecondTimeState::getInstance());
  }
}
} // namespace ls
