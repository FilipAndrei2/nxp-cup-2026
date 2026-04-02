
#pragma once
#include "utils/lifesource.hpp"
#include "dp/TSingleton.hpp"
#include "dto/DrivingCommandDTO.hpp"
#include "track_states/ATrackState.hpp"
#include "params/speed.hpp"

namespace ls {
class OnTrackState : public Singleton<OnTrackState>, public ATrackState {
  friend class Singleton<OnTrackState>;

public:
  virtual void updateNextState(ATrackStateContext &ctx) const override;

  virtual int MAX_SPEED() const override { return isInCrossway ? Speed::_4_WAY_CROSSWAY_SPEED : Speed::MAX; }
};
} // namespace ls
