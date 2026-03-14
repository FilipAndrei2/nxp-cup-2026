#include "car/ServoControllerImpl.hpp"
#include "params/Params.hpp"

namespace ls {

ServoControllerImpl::ServoControllerImpl() {
  ServoInit(Params::SERVO_PWM_CHANNEL, Params::SERVO_MAX_DUTY_CYCLE,
            Params::SERVO_MIN_DUTY_CYCLE, Params::SERVO_MED_DUTY_CYCLE);
}

void ServoControllerImpl::steer(ls::angle_t angle) {}
} // namespace ls
