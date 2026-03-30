#pragma once
#include "dp/TSingleton.hpp"
#include "dto/DrivingCommandDTO.hpp"
#include "track_states/ATrackState.hpp"
#include "utils/lifesource.hpp"

namespace ls {
class WaitingCubeState : public Singleton<WaitingCubeState>,
                         public ATrackState {
  friend class Singleton<WaitingCubeState>;

public:
  virtual void updateNextState(ATrackStateContext &ctx) const override;
  virtual speed_t computeSpeed(angle_t angle,
                               proximity_t cubeProximity) override;
};
} // namespace ls
