#include "car/ServoControllerImpl.hpp"
#include "math/Angles.hpp"
#include "params/Params.hpp"
#include "servo.h"
#include "utils/funcs.hpp"

namespace ls {

static void initServo() {
	ServoInit(Params::SERVO_PWM_CHANNEL, Params::SERVO_MAX_DUTY_CYCLE, Params::SERVO_MIN_DUTY_CYCLE, Params::SERVO_MED_DUTY_CYCLE);
	delay(Params::SERVO_INIT_DELAY);
}

ServoControllerImpl::ServoControllerImpl() {
	initServo();
}

void ServoControllerImpl::steer(ls::angle_t angle) {
  auto direction = Angles::angleToDir(angle);
  Steer(direction);
}
} // namespace ls
