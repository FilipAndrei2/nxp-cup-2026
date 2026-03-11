#include "car/AlgorithmStrategyImpl.hpp"

namespace ls {

AlgorithmStrategyImpl::AlgorithmStrategyImpl() {}

const ls::DrivingCommandDTO
AlgorithmStrategyImpl::computeParameters(const SensorDataDTO &sensorData) {
  auto res = this->getState().computeCommand(sensorData, *this);
  this->getState().updateNextState(sensorData, *this);

  return res;
}
} // namespace ls
