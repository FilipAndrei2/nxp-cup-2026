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

    // Translate
    int16_t  x = (v.x0 < v.x1) ? v.x1 - v.x0 : v.x0 - v.x1; // trebuie sa scad x mai mic din x mai mare
    uint16_t y = (v.y0 < v.y1) ? v.y0 - v.y1 : v.y1 - v.y0; // si sa scad y mai mare din y mai mic 
    
    if (x == 0 && y == 0) {
      continue; // skip degenerate zero-length segments
    }
    
    res->emplace_back((float)x, (float)y);
    res->back().normalize();
  }

  return res;
}

} // namespace ls
