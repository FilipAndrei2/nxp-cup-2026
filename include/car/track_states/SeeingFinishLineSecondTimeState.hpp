

#pragma once
#include "car/ITrackState.hpp"
#include "dp/TSingleton.hpp"
#include "dto/DrivingCommandDTO.hpp"
#include "lifesource.hpp"

namespace ls {
class SeeingFinishLineSecondTimeState
    : public Singleton<SeeingFinishLineSecondTimeState>,
      public ITrackState {
  friend class Singleton<SeeingFinishLineSecondTimeState>;

public:
  virtual const ls::DrivingCommandDTO
  computeCommand(const ls::SensorDataDTO &sensorData,
                 ATrackStateContext &ctx) override;
};
} // namespace ls
