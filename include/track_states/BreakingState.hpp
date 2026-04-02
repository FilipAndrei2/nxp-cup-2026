#pragma once
#include "dp/TSingleton.hpp"
#include "track_states/ATrackState.hpp"
#include "params/speed.hpp"

namespace ls {
class BreakingState : public Singleton<BreakingState>, public ATrackState {

  friend class Singleton<BreakingState>;

public:
  virtual void updateNextState(ATrackStateContext &ctx) const override;

protected:
  virtual int MAX_SPEED() const override { return Speed::BREAK; }
  virtual int computeSteer() override { return 0; };
  virtual speed_t computeSpeed(const int steer, const proximity_t inCubeProxi) override { return this->MAX_SPEED(); };
};
} // namespace ls
