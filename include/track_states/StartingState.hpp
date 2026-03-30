#pragma once
#include "dp/TSingleton.hpp"
#include "track_states/ATrackState.hpp"
#include "track_states/ITrackState.hpp"
#include "utils/lifesource.hpp"

namespace ls {
class StartingState : public ATrackState, public Singleton<StartingState> {

  friend class Singleton<StartingState>;

public:
  ~StartingState() = default;

  virtual void updateNextState(ATrackStateContext &) const override final;

protected:
  virtual speed_t computeSpeed(const angle_t inAngle,
                               const proximity_t inCubeProxi) override final;

private:
  StartingState() = default;
};
} // namespace ls
