#include "track_states/StartingBeforeFinishLineState.hpp"
#include "math/Vectors.hpp"
#include "params/speed.hpp"
#include "track_states/ATrackStateContext.hpp"
#include "track_states/SeeingFinishFirstTimeState.hpp"

namespace ls {
const ls::DrivingCommandDTO StartingBeforeFinishLineState::computeCommand(
    const ls::SensorDataDTO &sensorData, ls::ATrackStateContext &ctx) {

  return DrivingCommandDTO{0, Speed::MAX, false};
}

void StartingBeforeFinishLineState::updateNextState(
    const ls::SensorDataDTO &sensorData, ATrackStateContext &ctx) const {
  // Schimbam stateul daca detectam linia de fin
  if (Vectors::seeingFinishLine(sensorData.vectors)) {
    ctx.setState(&SeeingFinishFirstState::getInstance());
  }
}
} // namespace ls
