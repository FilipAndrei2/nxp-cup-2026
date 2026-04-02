#include "track_states/StartingState.hpp"
#include "track_states/SeeingFirstFinishState.hpp"
#include "track_states/ATrackStateContext.hpp"

namespace ls {
    void StartingState::updateNextState(ATrackStateContext & ctx) const  {
        // Schimbam stateul daca detectam linia de fin
        if (seeFinishLine) {
            ctx.setState(&SeeingFirstFinishState::getInstance());
        }
    }
}
