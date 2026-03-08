#pragma once
#include "lifesource.hpp"

#include "car/ATrackStateContext.hpp"
#include "dto/DrivingCommandDTO.hpp"
#include "dto/SensorDataDTO.hpp"

namespace ls {
class ATrackStateContext;
class ITrackState {
public:
  virtual ~ITrackState() = 0;

  virtual const ls::DrivingCommandDTO
  computeCommand(const ls::SensorDataDTO &sensorData,
                 ATrackStateContext &ctx) = 0;
};
} // namespace ls
