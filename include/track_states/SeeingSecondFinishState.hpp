

#pragma once
#include "dp/TSingleton.hpp"
#include "dto/DrivingCommandDTO.hpp"
#include "track_states/ATrackState.hpp"
#include "utils/lifesource.hpp"
#include "params/speed.hpp"

namespace ls {
class SeeingSecondFinishState : public Singleton<SeeingSecondFinishState>,
                                public ATrackState {
  friend class Singleton<SeeingSecondFinishState>;

public:
  virtual int MAX_SPEED() const override { return Speed::WAITING_CUBE_SPEED; }
  virtual void updateNextState(ATrackStateContext &ctx) const override;
};
} // namespace ls
