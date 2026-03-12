#pragma once
#include "dp/TSingleton.hpp"
#include "dto/DrivingCommandDTO.hpp"
#include "track_states/ITrackState.hpp"
#include "utils/lifesource.hpp"

namespace ls {
class StoppedState : public Singleton<StoppedState>, public ITrackState {
  friend class Singleton<StoppedState>;

public:
  virtual const ls::DrivingCommandDTO
  computeCommand(const SensorDataDTO &sensorData,
                 ATrackStateContext &ctx) override;
  virtual void updateNextState(const SensorDataDTO &sensorData,
                               ATrackStateContext &ctx) const override;
};
} // namespace ls
