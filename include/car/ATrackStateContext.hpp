#pragma once
#include "car/ITrackState.hpp"
#include "car/track_states/StartingBeforeFinishLineState.hpp"
#include "lifesource.hpp"

namespace ls {
class ITrackState;
class ATrackStateContext {

public:
  void setState(ITrackState &state) { this->state = state; }

protected:
  ATrackStateContext() = default;

private:
  ITrackState &state = StartingBeforeFinishLineState::getInstance();
};
} // namespace ls
