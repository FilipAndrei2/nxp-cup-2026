

#pragma once
#include "dp/TSingleton.hpp"
#include "dto/DrivingCommandDTO.hpp"
#include "track_states/ATrackState.hpp"
#include "track_states/ITrackState.hpp"
#include "utils/lifesource.hpp"

namespace ls {
class SeeingSecondFinishState : public Singleton<SeeingSecondFinishState>,
                                public ATrackState {
  friend class Singleton<SeeingSecondFinishState>;

public:
  virtual speed_t computeSpeed(const ls::angle_t angle,
                               ls::proximity_t cubeProximity) override;
  virtual void updateNextState(ATrackStateContext &ctx) const override;
};
} // namespace ls
