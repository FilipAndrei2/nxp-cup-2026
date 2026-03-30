#pragma once
#include "dto/DrivingCommandDTO.hpp"
#include "dto/SensorDataDTO.hpp"

namespace ls {
class ATrackStateContext;
class ITrackState {
public:
  virtual ~ITrackState();

  virtual const ls::DrivingCommandDTO
  computeCommand(const ls::SensorDataDTO &sensorData) = 0;

  virtual void updateNextState(ATrackStateContext &ctx) const = 0;

protected:
};
} // namespace ls
