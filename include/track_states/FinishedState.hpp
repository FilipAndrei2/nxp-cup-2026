#pragma once
#include "dp/TSingleton.hpp"
#include "dto/DrivingCommandDTO.hpp"
#include "track_states/ATrackState.hpp"
#include "utils/lifesource.hpp"

namespace ls {
class FinishedState : public Singleton<FinishedState>, public ATrackState {
  friend class Singleton<FinishedState>;

public:
  virtual void updateNextState(ATrackStateContext &ctx) const override;

protected:
  virtual angle_t computeAngle() override;
  virtual speed_t computeSpeed(angle_t angle,
                               proximity_t cubeProximity) override;
  virtual bool shouldStopCar() override;
};
} // namespace ls
