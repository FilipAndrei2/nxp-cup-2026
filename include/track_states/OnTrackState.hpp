
#pragma once
#include "dp/TSingleton.hpp"
#include "dto/DrivingCommandDTO.hpp"
#include "track_states/ATrackState.hpp"
#include "utils/lifesource.hpp"

namespace ls {
class OnTrackState : public Singleton<OnTrackState>, public ATrackState {
  friend class Singleton<OnTrackState>;

public:
  virtual void updateNextState(ATrackStateContext &ctx) const override;

protected:
  speed_t virtual computeSpeed(const angle_t inAngle,
                               const proximity_t inCubeProxi) override final;
};
} // namespace ls
