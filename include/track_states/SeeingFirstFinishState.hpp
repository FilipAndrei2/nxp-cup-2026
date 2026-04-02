#pragma once
#include "dp/TSingleton.hpp"
#include "dto/DrivingCommandDTO.hpp"
#include "track_states/ATrackState.hpp"
#include "utils/lifesource.hpp"
#include "params/speed.hpp"

namespace ls {
class SeeingFirstFinishState : public Singleton<SeeingFirstFinishState>,
                               public ATrackState {
  friend class Singleton<SeeingFirstFinishState>;

public:
  virtual void updateNextState(ATrackStateContext &ctx) const override;

  virtual int MAX_SPEED() const {
    return Speed::MAX;
  }

protected:
};
} // namespace ls
