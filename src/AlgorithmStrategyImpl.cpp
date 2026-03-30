#include "algorithm/AlgorithmStrategyImpl.hpp"

namespace ls {

const ls::DrivingCommandDTO
AlgorithmStrategyImpl::computeParameters(const SensorDataDTO &sensorData) {
  auto res = this->getState().computeCommand(sensorData);
  this->getState().updateNextState(*this);

  return res;
}
} // namespace ls
