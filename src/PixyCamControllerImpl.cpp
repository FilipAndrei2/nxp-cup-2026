#include "car/PixyCamControllerImpl.hpp"
#include "params/Params.hpp"
#include "pixy2.h"

namespace ls {
PixyCamControllerImpl::PixyCamControllerImpl() {
  Pixy2Init(Params::PIXY2_I2C_ADDRESS, Params::PIXY2_I2C_CHANNEL);
}

std::shared_ptr<std::vector<FVector2>> getVectors() {
  using namespace std;

  DetectedVectors dv;
  Pixy2GetVectors(&dv);
  auto res = make_shared<vector<FVector2>>();

  if (dv.NumberOfVectors == 0) {
    return nullptr;
  }

  for (auto i = 0; i < dv.NumberOfVectors; ++i) {
    Vector v = dv.Vectors[i];
    uint16_t x, y;
    if (v.y0 < v.y1) {
      y = v.y1 - v.y0;
      if (v.x0 < v.x1) {
        x = v.x1 - v.x0;
      } else {
        x = v.x0 - v.x1;
      }
    } else { // v.y1 < v.y0
      y = v.y0 - v.y1;
      if (v.x0 < v.x1) {
        x = v.x1 - v.x0;
      } else {
        x = v.x0 - v.x1;
      }
    }
    res->emplace_back((float)x, (float)y);
    (*res)[i].normalize();
  }

  return res;
}

} // namespace ls
