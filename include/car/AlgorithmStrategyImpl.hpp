#pragma once
#include "car/ATrackStateContext.hpp"
#include "car/IAlgorithmStrategy.hpp"
#include "car/ITrackState.hpp"
#include "lifesource.hpp"

#include "dp/TSingleton.hpp"

namespace ls {

class AlgorithmStrategyImpl : public ATrackStateContext,
                              public IAlgorithmStrategy,
                              public Singleton<AlgorithmStrategyImpl> {
  friend class Singleton<AlgorithmStrategyImpl>;

public:
  virtual ~AlgorithmStrategyImpl() override;
  virtual const ls::DrivingCommandDTO
  computeParameters(const SensorDataDTO &sensorData) override;

private:
  AlgorithmStrategyImpl();
};
} // namespace ls
