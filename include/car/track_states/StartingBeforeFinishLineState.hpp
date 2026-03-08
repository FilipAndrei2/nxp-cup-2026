#pragma once
#include "car/ITrackState.hpp"
#include "dp/TSingleton.hpp"
#include "lifesource.hpp"

namespace ls {
class StartingBeforeFinishLineState
    : public ITrackState,
      public Singleton<StartingBeforeFinishLineState> {

  friend class Singleton<StartingBeforeFinishLineState>;

public:
  ~StartingBeforeFinishLineState() = default;

  virtual const ls::DrivingCommandDTO
  computeCommand(const ls::SensorDataDTO &sensorData,
                 ATrackStateContext &ctx) override;

private:
  StartingBeforeFinishLineState() = default;
};
} // namespace ls
