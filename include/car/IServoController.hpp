#pragma once
#include "utils/lifesource.hpp"

namespace ls {

class IServoController {
public:
  virtual void steer(int angle) = 0;
  virtual ~IServoController() {}
};
} // namespace ls
