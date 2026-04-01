#include "car/SpeedControllerImpl.hpp"
#include "car/PixyCamControllerImpl.hpp"
#include "esc.h"
#include "params/Params.hpp"
#include "utils/funcs.hpp"

namespace ls {
static void initEsc () {

  EscInit(Params::ESC_PWM_CHANNEL, Params::ESC_MIN_DUTY_CYCLE, Params::ESC_MED_DUTY_CYCLE, Params::ESC_MAX_DUTY_CYCLE);
  PixyCamControllerImpl::changeLedColor(0, 0, 255);
  delay(Params::ESC_INIT_DELAY);
}


SpeedControllerImpl::SpeedControllerImpl() {
  initEsc();
}

void SpeedControllerImpl::changeSpeed(ls::speed_t speed) { EscSetSpeed(speed); }
} // namespace ls
