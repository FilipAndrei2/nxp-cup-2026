#pragma once
#include "car/ITrackState.hpp"
#include "dp/TSingleton.hpp"
#include "dto/DrivingCommandDTO.hpp"
#include "lifesource.hpp"

namespace ls {
class SeeingFinishFirstState : public Singleton<SeeingFinishFirstState>,
                               public ITrackState {
  friend class Singleton<SeeingFinishFirstState>;

public:
  virtual const ls::DrivingCommandDTO
  computeCommand(const ls::SensorDataDTO &sensorData,
                 ATrackStateContext &ctx) override;
};
} // namespace ls
