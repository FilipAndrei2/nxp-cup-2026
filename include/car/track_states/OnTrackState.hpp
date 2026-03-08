
#pragma once
#include "car/ITrackState.hpp"
#include "dp/TSingleton.hpp"
#include "dto/DrivingCommandDTO.hpp"
#include "lifesource.hpp"

namespace ls {
class OnTrackState : public Singleton<OnTrackState>, public ITrackState {
  friend class Singleton<OnTrackState>;

public:
  virtual const ls::DrivingCommandDTO
  computeCommand(const ls::SensorDataDTO &sensorData,
                 ATrackStateContext &ctx) override;
};
} // namespace ls
