#pragma once
#include "car/IServoController.hpp"
#include "utils/lifesource.hpp"
#include <dp/TSingleton.hpp>

namespace ls {
class ServoControllerImpl : public IServoController,
                            public Singleton<ServoControllerImpl> {
  friend class Singleton<ServoControllerImpl>;

public:
  virtual ~ServoControllerImpl() = default;
  virtual void steer(ls::angle_t angle) override;

private:
  ServoControllerImpl();
};
} // namespace ls
