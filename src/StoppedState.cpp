#include "track_states/StoppedState.hpp"
#include "dto/SensorDataDTO.hpp"
#include "track_states/ATrackStateContext.hpp"

namespace ls {

const DrivingCommandDTO
StoppedState::computeCommand(const SensorDataDTO &sensorData,
                             ATrackStateContext &ctx) {
  // nu mai miscam 1 centilitru
  return DrivingCommandDTO{.angle = 0.0f, .speed = 0, .shouldStop = true};
}

void StoppedState::updateNextState(const SensorDataDTO &sensorData,
                                   ATrackStateContext &ctx) const {
  /* Nu mai iesim din stateul asta :)
   * IDEA: Fa sa se aprinda un led pe pixy cand se termina, sau ceva
   */
}

} // namespace ls
