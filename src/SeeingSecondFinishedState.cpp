#include "track_states/SeeingSecondFinishState.hpp"
#include "track_states/WaitingCubeState.hpp"
#include "track_states/ATrackStateContext.hpp"

namespace ls {
    void SeeingSecondFinishState::updateNextState(ATrackStateContext &ctx) const {
    // schimba stateul daca nu mai vezi fin, sau daca incepem sa percepem cubul
    if (!this->seeFinishLine) {
      ctx.setState(&WaitingCubeState::getInstance());
    }
  }
}