
#pragma once
#include "dp/TSingleton.hpp"
#include "dto/DrivingCommandDTO.hpp"
#include "track_states/ATrackState.hpp"
#include "utils/lifesource.hpp"
#include "params/speed.hpp"
#include "track_states/SeeingSecondFinishState.hpp"

namespace ls {
class OnTrackState : public Singleton<OnTrackState>, public ATrackState {
  friend class Singleton<OnTrackState>;

public:
  virtual void updateNextState(ATrackStateContext &ctx) const override {
    // Schimbam stateul cand detectam linia de finish
    if (this->seeFinishLine) {
      ctx.setState(&SeeingSecondFinishState::getInstance());
    }
  }

  virtual int MAX_SPEED() const override { return isInCrossway ? Speed::_4_WAY_CROSSWAY_SPEED : Speed::MAX; }

};
} // namespace ls
