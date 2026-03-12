

#pragma once
#include "dp/TSingleton.hpp"
#include "dto/DrivingCommandDTO.hpp"
#include "track_states/ITrackState.hpp"
#include "utils/lifesource.hpp"

namespace ls {
class SeeingFinishLineSecondTimeState
    : public Singleton<SeeingFinishLineSecondTimeState>,
      public ITrackState {
  friend class Singleton<SeeingFinishLineSecondTimeState>;

public:
  virtual const ls::DrivingCommandDTO
  computeCommand(const ls::SensorDataDTO &sensorData,
                 ATrackStateContext &ctx) override;

  virtual void updateNextState(const ls::SensorDataDTO &sensorData,
                               ATrackStateContext &ctx) const override;
};
} // namespace ls
