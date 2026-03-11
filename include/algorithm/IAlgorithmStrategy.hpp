#pragma once
#include "dto/DrivingCommandDTO.hpp"
#include "dto/SensorDataDTO.hpp"
#include "utils/lifesource.hpp"

namespace ls {
class IAlgorithmStrategy {
public:
  virtual ~IAlgorithmStrategy() = default;
  virtual const ls::DrivingCommandDTO
  computeParameters(const SensorDataDTO &sensorData) = 0;
};
} // namespace ls
