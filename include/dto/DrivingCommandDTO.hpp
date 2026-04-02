#pragma once
#include "utils/lifesource.hpp"

namespace ls {
struct DrivingCommandDTO {
  int steer;
  speed_t speed;
  bool shouldStop;
};
} // namespace ls
