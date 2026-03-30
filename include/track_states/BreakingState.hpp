#pragma once
#include "dp/TSingleton.hpp"
#include "track_states/ATrackState.hpp"

namespace ls {
class BreakingState : public Singleton<BreakingState>, public ATrackState {

  friend class Singleton<BreakingState>;

public:
  virtual void updateNextState(ATrackStateContext &ctx) const override;

protected:
  virtual angle_t computeAngle() override;
  virtual speed_t computeSpeed(const angle_t inAngle,
                               const proximity_t inCubeProxi) override;
};
} // namespace ls
