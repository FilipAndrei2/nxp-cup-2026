#include "track_states/SeeingFinishLineSecondTimeState.hpp"
#include "params/speed.hpp"
#include "track_states/ATrackStateContext.hpp"
#include "track_states/WaitingToAproachCubeState.hpp"

namespace ls {

const ls::DrivingCommandDTO SeeingFinishLineSecondTimeState::computeCommand(
    const ls::SensorDataDTO &sensorData, ATrackStateContext &ctx) {
  return DrivingCommandDTO{.angle = TODO(),
                           .speed = Speed::WAITING_CUBE_SPEED,
                           false}; // todo: finish
}

void SeeingFinishLineSecondTimeState::updateNextState(
    const ls::SensorDataDTO &sensorData, ATrackStateContext &ctx) const {
  // schimba stateul daca nu mai vezi fin
  if (Vectors::notSeeingFinishLine(sensorData.vectors)) {
    ctx.setState(&WaitingToApproachCubeState::getInstance());
  }
}
} // namespace ls
