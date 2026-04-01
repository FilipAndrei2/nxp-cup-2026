#pragma once
#include "track_states/ITrackState.hpp"
#include "track_states/StartingState.hpp"
#include "utils/lifesource.hpp"

namespace ls {
class ITrackState;
class ATrackStateContext {

public:
virtual void setState(ITrackState* state) {
  if (state == nullptr) {
	  throw std::runtime_error("ATrackStateContext: setState");
  }
  this->state = state;
}

protected:
  ATrackStateContext() = default;
  ITrackState &getState() { return *state; }

private:
  ITrackState *state = nullptr;
};
} // namespace ls
