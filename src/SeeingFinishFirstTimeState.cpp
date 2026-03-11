#include "track_states/SeeingFinishFirstTimeState.hpp"
#include "math/Vectors.hpp"
#include "params/speed.hpp"
#include "track_states/ATrackStateContext.hpp"
#include "track_states/OnTrackState.hpp"
#include "utils/lifesource.hpp"

namespace ls {
const ls::DrivingCommandDTO
SeeingFinishFirstState::computeCommand(const ls::SensorDataDTO &sensorData,
                                       ATrackStateContext &ctx) {
  using namespace ls;
  updateNextState(sensorData, ctx);
  return DrivingCommandDTO{
      .angle = 0.0f, .speed = Speed::MAX, .shouldStop = false};
}

void SeeingFinishFirstState::updateNextState(
    const ls::SensorDataDTO &sensorData, ATrackStateContext &ctx) const {
  if (seeFinishLine(sensorData)) {
    ctx.setState(&OnTrackState::getInstance());
  }
}

bool SeeingFinishFirstState::seeFinishLine(
    const ls::SensorDataDTO &sensorData) const {
  for (auto &vector : sensorData.vectors) {
    if (Vectors::isFinishLine(vector)) {
      return true;
    }
  }
  return false;
}

} // namespace ls
