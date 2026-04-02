#pragma once
#include "car/IServoController.hpp"
#include <dp/TSingleton.hpp>
#include "utils/lifesource.hpp"

namespace ls {
class ServoControllerImpl : public IServoController,
                            public Singleton<ServoControllerImpl> {
  friend class Singleton<ServoControllerImpl>;

public:
  virtual ~ServoControllerImpl() = default;
  virtual void steer(int dir) override;

private:
  ServoControllerImpl();
};
} // namespace ls
