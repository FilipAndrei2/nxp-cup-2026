#include "car/EngineControllerImpl.hpp"
#include "esc.h"
#include "params/Params.hpp"

namespace ls {

SpeedControllerImpl::SpeedControllerImpl() {
  EscInit(Params::ESC_PWM_CHANNEL, Params::ESC_MIN_DUTY_CYCLE,
          Params::ESC_MED_DUTY_CYCLE, Params::ESC_MAX_DUTY_CYCLE);
}

void SpeedControllerImpl::changeSpeed(ls::speed_t speed) { EscSetSpeed(speed); }
} // namespace ls
