#pragma once
#include "dp/TSingleton.hpp"
#include "dto/DrivingCommandDTO.hpp"
#include "track_states/ATrackState.hpp"
#include "track_states/BreakingState.hpp"
#include "utils/lifesource.hpp"
#include "params/speed.hpp"

namespace ls {
class WaitingCubeState : public Singleton<WaitingCubeState>,
                         public ATrackState {
  friend class Singleton<WaitingCubeState>;

public:
  virtual void updateNextState(ATrackStateContext &ctx) const override {
  // Schimbam starea pe STOP daca cubul este suficient de aproape.
  if (this->cubeDetected) {
    ctx.setState(&BreakingState::getInstance());
  }
}

protected:
  virtual speed_t MAX_SPEED() const override { return Speed::WAITING_CUBE_SPEED; }
  virtual speed_t computeSpeed(int steer, proximity_t cubeProximity) override { return this->MAX_SPEED(); }
};
} // namespace ls
