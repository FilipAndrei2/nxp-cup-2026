#include "track_states/SeeingFirstFinishState.hpp"
#include "track_states/ATrackStateContext.hpp"
#include "track_states/OnTrackState.hpp"

namespace ls {
  
void SeeingFirstFinishState::updateNextState(ATrackStateContext &ctx) const {
  if (!seeFinishLine) {
    ctx.setState(&OnTrackState::getInstance());
  }
}

} // namespace  ls
