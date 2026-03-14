#include "car/ServoControllerImpl.hpp"
#include "math/Angles.hpp"
#include "params/Params.hpp"
#include "servo.h"

namespace ls {

ServoControllerImpl::ServoControllerImpl() {
  ServoInit(Params::SERVO_PWM_CHANNEL, Params::SERVO_MAX_DUTY_CYCLE,
            Params::SERVO_MIN_DUTY_CYCLE, Params::SERVO_MED_DUTY_CYCLE);
}

void ServoControllerImpl::steer(ls::angle_t angle) {
  auto direction = Angles::angleToDir(angle);
  Steer(direction);
}
} // namespace ls
