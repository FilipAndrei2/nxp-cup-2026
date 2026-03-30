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
    int16_t dx, dy;
    if (v.y0 >= v.y1) {
      // (x1,y1) is the head (top of image = forward); y decreases upward in
      // image coords, so dy = y0 - y1 gives a positive forward component.
      dy = (int16_t)v.y0 - (int16_t)v.y1;
      dx = (int16_t)v.x1 - (int16_t)v.x0;
    } else {
      // Flip so that the forward (y) component is always positive.
      dy = (int16_t)v.y1 - (int16_t)v.y0;
      dx = (int16_t)v.x0 - (int16_t)v.x1;
    }
    res->emplace_back((float)dx, (float)dy);
    (*res)[i].normalize();
  }

  return res;
}

} // namespace ls
