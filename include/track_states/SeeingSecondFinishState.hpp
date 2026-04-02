

#pragma once
#include "dp/TSingleton.hpp"
#include "dto/DrivingCommandDTO.hpp"
#include "track_states/ATrackState.hpp"
#include "track_states/ITrackState.hpp"
#include "track_states/WaitingCubeState.hpp"
#include "utils/lifesource.hpp"

namespace ls {
class SeeingSecondFinishState : public Singleton<SeeingSecondFinishState>,
                                public ATrackState {
  friend class Singleton<SeeingSecondFinishState>;

public:
  virtual speed_t MAX_SPEED() const override { return Speed::WAITING_CUBE_SPEED; }
  virtual void updateNextState(ATrackStateContext &ctx) const override{
    // schimba stateul daca nu mai vezi fin, sau daca incepem sa percepem cubul
    if (!this->seeFinishLine) {
      ctx.setState(&WaitingCubeState::getInstance());
    }
  }
};
} // namespace ls
