#pragma once
#include "dp/TSingleton.hpp"
#include "track_states/ITrackState.hpp"
#include "utils/lifesource.hpp"

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

  virtual void updateNextState(const ls::SensorDataDTO &,
                               ATrackStateContext &) const override;

private:
  StartingBeforeFinishLineState() = default;
};
} // namespace ls
