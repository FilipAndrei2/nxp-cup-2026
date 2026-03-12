#pragma once
#include "math/Vector2.hpp"
#include "params/Params.hpp"
#include <cmath>

namespace ls {
class Vectors {
public:
  static constexpr float HORIZONTAL_THRESHOLD = 0.1;

  template <class T> static bool isFinishLine(const Vector2<T> &vector) {
    return std::abs(vector.getY()) <= HORIZONTAL_THRESHOLD;
  }

  /**
   * @brief Returneaza numarul de vectori linie de start
   *
   * Un vector linie de start este un vector orizontal (coordonata y ~= 0)
   *
   * @param vectors Vectorii care sunt verificati daca sunt orizontali
   * @returns Numarul de vectori linie de start (0 inseamna ca nu este detectata
   * linia de start)
   */
  static uint8_t
  numberOfFinishLineVectors(const std::vector<FVector2> &vectors) {
    if (vectors.size() == 0) {
      return 0;
    }
    uint8_t numberOfHorizVect = 0;
    for (auto &vect : vectors) {
      if (Vectors::isFinishLine(vect)) {
        numberOfHorizVect++;
      }
    }
    return numberOfHorizVect;
  }

  static bool notSeeingFinishLine(const std::vector<FVector2> &vectors) {
    return numberOfFinishLineVectors(vectors) == 0;
  }

  static bool seeingFinishLine(const std::vector<FVector2> &vectors) {
    return (numberOfFinishLineVectors(vectors) >=
            Params::FINISH_LINE_NUMBER_OF_VECTORS_THSH);
  }

  static const FVector2 ZERO;
};
} // namespace ls
