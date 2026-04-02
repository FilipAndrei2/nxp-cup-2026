#pragma once
#include "dp/TSingleton.hpp"
#include "dto/DrivingCommandDTO.hpp"
#include "track_states/ATrackState.hpp"
#include "utils/lifesource.hpp"
#include "params/speed.hpp"
namespace ls {
class FinishedState : public Singleton<FinishedState>, public ATrackState {
  friend class Singleton<FinishedState>;

public:
  virtual void updateNextState(ATrackStateContext &ctx) const override;

protected:
  virtual int MAX_SPEED() const { return 0; }
  virtual int computeSteer() override { return 0; };
  virtual speed_t computeSpeed(int steer, proximity_t cubeProximity) override { return MAX_SPEED(); };
  virtual bool shouldStopCar() override { return true; }
};
} // namespace ls
