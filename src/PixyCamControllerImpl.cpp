/**
 * @file PixyCamControllerImpl.cpp
 * @brief Implementarea controllerului camerei Pixy2.
 */
#include "car/PixyCamControllerImpl.hpp"
#include "params/Params.hpp"
#include "pixy2.h"

namespace ls {

/**
 * @brief Inițializează camera Pixy2 prin interfața I2C.
 *
 * Utilizează adresa I2C și canalul definite în @c Params.
 */
PixyCamControllerImpl::PixyCamControllerImpl() {
  Pixy2Init(Params::PIXY2_I2C_ADDRESS, Params::PIXY2_I2C_CHANNEL);
}

/**
 * @brief Returnează vectorii de direcție detectați de camera Pixy2.
 *
 * Apelează driverul Pixy2 pentru a obține vectorii detectați, calculează
 * diferențele de coordonate și normalizează fiecare vector rezultat.
 *
 * @return Un pointer partajat la lista vectorilor normalizați,
 *         sau @c nullptr dacă nu s-a detectat niciun vector.
 */
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
