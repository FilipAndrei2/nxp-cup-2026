#pragma once
#include "car/ISpeedController.hpp"
#include "dp/TSingleton.hpp"
#include "utils/lifesource.hpp"

namespace ls {
class SpeedControllerImpl : public ISpeedController,
                            public Singleton<SpeedControllerImpl> {
  friend class Singleton<SpeedControllerImpl>;

public:
  virtual void changeSpeed(ls::speed_t speed) override;
  virtual ~SpeedControllerImpl() = default;

private:
  SpeedControllerImpl();
};
} // namespace ls
