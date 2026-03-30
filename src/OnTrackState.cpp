#include "track_states/OnTrackState.hpp"
#include "params/Params.hpp"
#include "params/speed.hpp"
#include "track_states/ATrackStateContext.hpp"
#include "track_states/SeeingFinishLineSecondTimeState.hpp"
#include <array>
#include "utils/lifesource.hpp"

#include "OnTrackStateImplFunctions.hpp"

namespace ls {

const ls::DrivingCommandDTO
OnTrackState::computeCommand(const ls::SensorDataDTO &sensorData) {
  std::array<FVector2, 4> infoVectors;
  uint8_t numberInfoVectors =
      filterTrackVectors(*sensorData.vectors, infoVectors);

  angle_t angle;
  speed_t speed;
  computeSpeedAndAngle(infoVectors, numberInfoVectors, angle, speed);
  return DrivingCommandDTO{.angle = angle, .speed = speed, .shouldStop = false};
}

static uint8_t filterTrackVectors(const std::vector<ls::FVector2> &vectors,
                                  std::array<ls::FVector2, 4> &outInfoVectors) {
  using namespace ls;

  uint8_t numberInfoVectors = 0;
  for (auto &v : vectors) {
    if (numberInfoVectors >= 4) {
      break;
    }
    if (!Vectors::isFinishLine(v) && v != Vectors::ZERO) {
      outInfoVectors[numberInfoVectors] = v;
      numberInfoVectors++;
    }
  }
  return numberInfoVectors;
}

static void computeSpeedAndAngle(const std::array<FVector2, 4> &inInfoVectors,
                                 uint8_t inNumberInfoVectors, angle_t &outAngle,
                                 speed_t &outSpeed) {
  switch (inNumberInfoVectors) {
  case 0: {
    // Daca nu vedem niciun vector, probabil e in intersectie cu 4 cai, mergem
    // inainte
    outAngle = 0.0f;
    outSpeed = Speed::_4_WAY_CROSSWAY_SPEED;
    break;
  }
  case 1: {
    // Pentru un vector, calculam unghiul intre el si vectorul nord, si scalam
    // viteza
    outAngle = Vector2<float>::AngleBetween(inInfoVectors[0], Vectors::NORTH);
    outSpeed = Speed::scale(Speed::MAX, outAngle);
    break;
  }
  case 2: {
    // Pentru 2 vectori:
    // 1. se calculeaza un vector mediu M intre ei
    // 2. se calculeaza unghiul dintre vectorul mediu M si vectorul NORD
    // 3. se scaleaza viteza in functie de unghi
    FVector2 medi = Vector2<float>::Avg(inInfoVectors[0], inInfoVectors[1]);
    outAngle = Vector2<float>::AngleBetween(medi, Vectors::NORTH);
    outSpeed = Speed::scale(Speed::MAX, outAngle);
    break;
  }
  default: {
    // Pentru 3 sau mai multi vectori:
    // Ii luam doar pe primi 2 si aplicam acelasi algoritm ca in cazul cu 2
    // vectori
    FVector2 medi = Vector2<float>::Avg(inInfoVectors[0], inInfoVectors[1]);
    outAngle = Vector2<float>::AngleBetween(medi, Vectors::NORTH);
    outSpeed = Speed::scale(Speed::MAX, outAngle);
    break;
  }
  }
}

void OnTrackState::updateNextState(const ls::SensorDataDTO &sensorData,
                                   ATrackStateContext &ctx) const {
  // Schimbam stateul cand detectam linia de finish
  if (Vectors::seeingFinishLine(*sensorData.vectors)) {
    ctx.setState(&SeeingFinishLineSecondTimeState::getInstance());
  }
}
} // namespace ls
