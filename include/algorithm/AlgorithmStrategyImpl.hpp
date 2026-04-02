#pragma once
#include "algorithm/IAlgorithmStrategy.hpp"
#include "dp/TSingleton.hpp"
#include "track_states/ATrackStateContext.hpp"
#include "track_states/ITrackState.hpp"
#include "utils/lifesource.hpp"
#include "track_states/StartingState.hpp"

namespace ls {

class AlgorithmStrategyImpl : public ATrackStateContext,
                              public IAlgorithmStrategy,
                              public Singleton<AlgorithmStrategyImpl> {
  friend class Singleton<AlgorithmStrategyImpl>;

public:
  virtual ~AlgorithmStrategyImpl() override = default;
  virtual const ls::DrivingCommandDTO
  computeParameters(const SensorDataDTO &sensorData) override;
  
  virtual void setInitialState() override {
    this->setState(&StartingState::getInstance());
  }
protected:
  AlgorithmStrategyImpl() = default;
};
} // namespace ls
