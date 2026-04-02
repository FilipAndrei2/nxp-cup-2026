#pragma once
#include "dp/TSingleton.hpp"
#include "dto/DrivingCommandDTO.hpp"
#include "track_states/ATrackState.hpp"
#include "track_states/OnTrackState.hpp"
#include "utils/lifesource.hpp"

namespace ls {
class SeeingFirstFinishState : public Singleton<SeeingFirstFinishState>,
                               public ATrackState {
  friend class Singleton<SeeingFirstFinishState>;

public:
  virtual const ls::DrivingCommandDTO
  computeCommand(const ls::SensorDataDTO &sensorData) override;

  virtual void updateNextState(ATrackStateContext &ctx) const override {
    if (!seeFinishLine) {
      ctx.setState(&OnTrackState::getInstance());
    }
  }

protected:
  virtual speed_t computeSpeed(int ,const proximity_t ) override final;
};
} // namespace ls
