#include "car/PixyCamControllerImpl.hpp"
#include "params/Params.hpp"
#include "pixy2.h"

namespace ls {
PixyCamControllerImpl::PixyCamControllerImpl() {
  Pixy2Init(Params::PIXY2_I2C_ADDRESS, Params::PIXY2_I2C_CHANNEL);
}

std::shared_ptr<std::vector<Vector2<float>>> PixyCamControllerImpl::getVectors() {
  using namespace std;

  DetectedVectors dv;
  Pixy2GetVectors(&dv);
  auto res = make_shared<vector<FVector2>>();

  if (dv.NumberOfVectors == 0) {
    return res;
  }

  for (auto i = 0; i < dv.NumberOfVectors; ++i) {
    Vector v = dv.Vectors[i];
    // Pixy2 uses screen coordinates where y increases downward, so y0
    // (the tail/bottom of the segment) has a numerically larger value than
    // y1 (the head/top). Translate to a direction vector pointing "up"
    // (toward the horizon) with a signed x so the car can distinguish
    // left from right.
    int16_t dx, dy;
    if (v.y0 > v.y1) {
      // Normal orientation: y0 is the lower point (numerically larger screen-y)
      dy = (int16_t)(v.y0 - v.y1);
      dx = (int16_t)(v.x1 - v.x0);
    } else {
      // Inverted orientation: y1 is the lower point
      dy = (int16_t)(v.y1 - v.y0);
      dx = (int16_t)(v.x0 - v.x1);
    }
    if (dx == 0 && dy == 0) {
      continue; // skip degenerate zero-length segments
    }
    res->emplace_back((float)dx, (float)dy);
    res->back().normalize();
  }

  return res;
}

} // namespace ls
