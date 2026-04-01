#include "car/ServoControllerImpl.hpp"
#include "math/Angles.hpp"
#include "params/Params.hpp"
#include "servo.h"

namespace ls {

ServoControllerImpl::ServoControllerImpl() {

}

void ServoControllerImpl::steer(ls::angle_t angle) {
  auto direction = Angles::angleToDir(angle);
  Steer(direction);
}
} // namespace ls
