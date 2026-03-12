
#pragma once
#include "dp/TSingleton.hpp"
#include "dto/DrivingCommandDTO.hpp"
#include "track_states/ITrackState.hpp"
#include "utils/lifesource.hpp"

namespace ls {
class OnTrackState : public Singleton<OnTrackState>, public ITrackState {
  friend class Singleton<OnTrackState>;

public:
  virtual const ls::DrivingCommandDTO
  computeCommand(const ls::SensorDataDTO &sensorData,
                 ATrackStateContext &ctx) override;
  virtual void updateNextState(const ls::SensorDataDTO &sensorData,
                               ATrackStateContext &ctx) const override;
};
} // namespace ls
