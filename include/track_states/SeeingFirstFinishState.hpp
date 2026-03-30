#pragma once
#include "dp/TSingleton.hpp"
#include "dto/DrivingCommandDTO.hpp"
#include "track_states/ATrackState.hpp"
#include "utils/lifesource.hpp"

namespace ls {
class SeeingFirstFinishState : public Singleton<SeeingFirstFinishState>,
                               public ATrackState {
  friend class Singleton<SeeingFirstFinishState>;

public:
  virtual const ls::DrivingCommandDTO
  computeCommand(const ls::SensorDataDTO &sensorData) override;

  virtual void updateNextState(ATrackStateContext &ctx) const override;

protected:
  virtual speed_t computeSpeed(const angle_t inAngle,
                               const proximity_t inCubeProxi) override final;
};
} // namespace ls
