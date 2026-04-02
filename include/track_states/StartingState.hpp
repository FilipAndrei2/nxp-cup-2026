#pragma once
#include "dp/TSingleton.hpp"
#include "track_states/ATrackState.hpp"
#include "utils/lifesource.hpp"
#include "params/speed.hpp"

namespace ls {
class StartingState : public ATrackState, public Singleton<StartingState> {

  friend class Singleton<StartingState>;

public:
  ~StartingState() = default;

  virtual void updateNextState(ATrackStateContext & ctx) const override;

protected:
  virtual int MAX_SPEED() const { return Speed::MAX; }

private:
  StartingState() = default;
};
} // namespace ls
