#pragma once
#include "dp/TSingleton.hpp"
#include "dto/DrivingCommandDTO.hpp"
#include "track_states/ITrackState.hpp"
#include "utils/lifesource.hpp"

namespace ls {
class WaitingToApproachCubeState : public Singleton<WaitingToApproachCubeState>,
                                   public ITrackState {
  friend class Singleton<WaitingToApproachCubeState>;

public:
  virtual const ls::DrivingCommandDTO
  computeCommand(const SensorDataDTO &sensorData,
                 ATrackStateContext &ctx) override;
};
} // namespace ls
