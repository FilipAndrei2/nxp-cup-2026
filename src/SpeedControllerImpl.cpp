#include "car/SpeedControllerImpl.hpp"
#include "car/PixyCamControllerImpl.hpp"
#include "esc.h"
#include "params/Params.hpp"

namespace ls {

SpeedControllerImpl::SpeedControllerImpl() {

  PixyCamControllerImpl::changeLedColor(0, 0, 255);

}

void SpeedControllerImpl::changeSpeed(ls::speed_t speed) { EscSetSpeed(speed); }
} // namespace ls
