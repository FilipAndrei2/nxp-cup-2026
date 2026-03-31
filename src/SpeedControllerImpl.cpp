#include "car/SpeedControllerImpl.hpp"
#include "esc.h"
#include "params/Params.hpp"

namespace ls {

SpeedControllerImpl::SpeedControllerImpl() {
  EscInit(Params::ESC_PWM_CHANNEL, Params::ESC_MIN_DUTY_CYCLE,
          Params::ESC_MED_DUTY_CYCLE, Params::ESC_MAX_DUTY_CYCLE);
  /* Defensive arming delay: ESC and MCU share the same battery, so they
   * power up simultaneously.  The ESC needs a sustained neutral signal
   * before it considers itself armed.  Without this delay the MCU boots
   * faster than the ESC's arming window, causing continuous beeping and
   * the motor refusing to respond.
   *
   * A busy-wait is intentional here: this constructor runs during single-
   * threaded startup (no RTOS, no other tasks), so blocking the CPU is
   * harmless and consistent with the rest of the initialisation code.
   * The volatile qualifier prevents the compiler from eliminating the loop.
   * Actual duration depends on CPU clock and optimisation level; increase
   * Params::ESC_ARM_DELAY_ITERATIONS if the ESC still beeps on startup. */
  volatile uint32_t armDelay = Params::ESC_ARM_DELAY_ITERATIONS;
  while (armDelay != 0U) {
    armDelay--;
  }
}

void SpeedControllerImpl::changeSpeed(ls::speed_t speed) { EscSetSpeed(speed); }
} // namespace ls
