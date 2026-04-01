#include <utils/funcs.hpp>
#include "utils/lifesource.hpp"
#include "main_functions.h"
#include "params/Params.hpp"

#include "esc.h"
#include "pixy2.h"
#include "servo.h"

/**
 * Program entry point
 */
int main(void) {
	using namespace ls;
  DriversInit();
  Pixy2Init(Params::PIXY2_I2C_ADDRESS, Params::PIXY2_I2C_CHANNEL);
  EscInit(Params::ESC_PWM_CHANNEL, Params::ESC_MIN_DUTY_CYCLE,
          Params::ESC_MED_DUTY_CYCLE, Params::ESC_MAX_DUTY_CYCLE);

  ServoInit(Params::SERVO_PWM_CHANNEL, Params::SERVO_MAX_DUTY_CYCLE,
            Params::SERVO_MIN_DUTY_CYCLE, Params::SERVO_MED_DUTY_CYCLE);

  EscSetSpeed(100);
  EscSetBrake(0);
  return ls::buildCar().run();
}
