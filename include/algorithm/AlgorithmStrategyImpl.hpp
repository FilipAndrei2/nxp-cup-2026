#pragma once
#include "algorithm/IAlgorithmStrategy.hpp"
#include "dp/TSingleton.hpp"
#include "track_states/ATrackStateContext.hpp"
#include "track_states/ITrackState.hpp"
#include "utils/lifesource.hpp"

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
