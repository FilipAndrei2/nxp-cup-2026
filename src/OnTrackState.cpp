#include "track_states/OnTrackState.hpp"
#include "track_states/ATrackStateContext.hpp"
#include "track_states/SeeingSecondFinishState.hpp"

namespace ls {
    void OnTrackState::updateNextState(ATrackStateContext &ctx) const  {
    // Schimbam stateul cand detectam linia de finish
    if (this->seeFinishLine) {
      ctx.setState(&SeeingSecondFinishState::getInstance());
    }
  }
}