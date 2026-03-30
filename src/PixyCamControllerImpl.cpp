#include "car/PixyCamControllerImpl.hpp"
#include "params/Params.hpp"
#include "pixy2.h"

namespace ls {
PixyCamControllerImpl::PixyCamControllerImpl() {
  Pixy2Init(Params::PIXY2_I2C_ADDRESS, Params::PIXY2_I2C_CHANNEL);
}

/**
 * Get raw vectors from Pixy Camera
 *
 * @return A collection of normalized vectors in the first and second quadrant
 * (x is signed, y >= 0)
 */
std::shared_ptr<std::vector<Vector2<float>>>
PixyCamControllerImpl::getVectors() {
  using namespace std;

  DetectedVectors dv;
  Pixy2GetVectors(&dv);
  auto res = make_shared<vector<FVector2>>();

  if (dv.NumberOfVectors == 0) {
    return res;
  }
  res->reserve(
      dv.NumberOfVectors /
      2); // Mini echilibru intre evitare de copiere si consum de memorie
          // Probabilitatea sa vada mai mult de NumberOfVectors/2 (50) vectori
          // este foarte mica, in general vede 2-3 maxim 5
  for (auto i = 0; i < dv.NumberOfVectors; ++i) {
    Vector v = dv.Vectors[i];
    int16_t x = (v.y0 > v.y1) ? (int16_t)(v.x1 - v.x0) : (int16_t)(v.x0 - v.x1);
    uint16_t y =
        (v.y0 > v.y1) ? (int16_t)(v.y0 - v.y1) : (int16_t)(v.y1 - v.y0);
    res->emplace_back((float)x, (float)y);
    (*res)[i].normalize();
  }

  return res;
}

} // namespace ls
