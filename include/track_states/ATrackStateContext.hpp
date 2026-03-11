#pragma once
#include "track_states/ITrackState.hpp"
#include "track_states/StartingBeforeFinishLineState.hpp"
#include "utils/lifesource.hpp"

namespace ls {
class ITrackState;
class ATrackStateContext {

public:
  void setState(ITrackState *state) { this->state = state; }

protected:
  ATrackStateContext() = default;
  ITrackState &getState() { return *state; }

private:
  ITrackState *state = &StartingBeforeFinishLineState::getInstance();
};
} // namespace ls
