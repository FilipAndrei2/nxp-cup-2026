#pragma once
#include "utils/lifesource.hpp"

#include "dto/DrivingCommandDTO.hpp"
#include "dto/SensorDataDTO.hpp"
#include "math/Vectors.hpp"

namespace ls {
class ATrackStateContext;
class ITrackState {
public:
  virtual ~ITrackState() = 0;

  virtual const ls::DrivingCommandDTO
  computeCommand(const ls::SensorDataDTO &sensorData,
                 ATrackStateContext &ctx) = 0;

  virtual void updateNextState(const ls::SensorDataDTO &sensorData,
                               ATrackStateContext &ctx) const = 0;

protected:
};
} // namespace ls
