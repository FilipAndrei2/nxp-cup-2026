#pragma once
#include "utils/lifesource.hpp"

namespace ls {

class ISpeedController {
public:
  // XXX: Range [-100, 100]
  // -100 -> reverse
  // 100 viteza maxima
  virtual void changeSpeed(ls::speed_t speed) = 0;
  virtual ~ISpeedController() = default;
};
} // namespace ls
