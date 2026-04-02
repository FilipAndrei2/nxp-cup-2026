#include "track_states/WaitingCubeState.hpp"
#include "track_states/ATrackStateContext.hpp"
#include "track_states/BreakingState.hpp"

namespace ls {
    void WaitingCubeState::updateNextState(ATrackStateContext &ctx) const {
        // Schimbam starea pe STOP daca cubul este suficient de aproape.
        if (this->cubeDetected) {
            ctx.setState(&BreakingState::getInstance());
        }
  }
}