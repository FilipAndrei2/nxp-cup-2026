#pragma once
#include "dp/TSingleton.hpp"
#include "dto/DrivingCommandDTO.hpp"
#include "track_states/ITrackState.hpp"
#include "utils/lifesource.hpp"

namespace ls {
class WaitingToAproachCubeState : public Singleton<WaitingToAproachCubeState>,
                                  public ITrackState {
  friend class Singleton<WaitingToAproachCubeState>;

public:
  virtual const ls::DrivingCommandDTO
  computeCommand(const SensorDataDTO &sensorData,
                 ATrackStateContext &ctx) override;
};
} // namespace ls
