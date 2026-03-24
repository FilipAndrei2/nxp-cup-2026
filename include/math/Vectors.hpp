/**
 * @file Vectors.hpp
 * @brief Utilitare pentru analiza și clasificarea vectorilor de linie.
 */
#pragma once
#include "math/Vector2.hpp"
#include "params/Params.hpp"
#include <cmath>

namespace ls {

/**
 * @brief Clasă utilitară pentru analiza vectorilor detectați de camera Pixy2.
 *
 * Oferă funcții statice pentru clasificarea vectorilor ca linii de finish,
 * contorizarea lor și verificarea vizibilității liniei de finish.
 */
class Vectors {
public:
  /// @brief Pragul sub care coordonata Y a unui vector îl califică drept orizontal.
  static constexpr float HORIZONTAL_THRESHOLD = 0.1;

  /**
   * @brief Verifică dacă un vector reprezintă linia de finish.
   *
   * Un vector este considerat linie de finish dacă este aproximativ orizontal
   * (valoarea absolută a componentei Y este sub @c HORIZONTAL_THRESHOLD).
   *
   * @tparam T Tipul componentelor vectorului.
   * @param vector Vectorul de verificat.
   * @return @c true dacă vectorul este orizontal (linie de finish).
   */
  template <class T> static bool isFinishLine(const Vector2<T> &vector) {
    return std::abs(vector.getY()) <= HORIZONTAL_THRESHOLD;
  }

  /**
   * @brief Returnează numărul de vectori care reprezintă linia de finish.
   *
   * Un vector linie de finish este un vector orizontal (coordonata Y ≈ 0).
   *
   * @param vectors Lista de vectori verificați.
   * @return Numărul de vectori orizontali detectați (0 înseamnă că linia
   *         de finish nu este vizibilă).
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

  /**
   * @brief Verifică dacă linia de finish nu este vizibilă.
   * @param vectors Lista de vectori detectați.
   * @return @c true dacă niciun vector nu este clasificat ca linie de finish.
   */
  static bool notSeeingFinishLine(const std::vector<FVector2> &vectors) {
    return numberOfFinishLineVectors(vectors) == 0;
  }

  /**
   * @brief Verifică dacă linia de finish este vizibilă.
   *
   * Returnează @c true dacă numărul de vectori orizontali atinge sau depășește
   * pragul definit în @c Params::FINISH_LINE_NUMBER_OF_VECTORS_THSH.
   *
   * @param vectors Lista de vectori detectați.
   * @return @c true dacă linia de finish este detectată cu suficienți vectori.
   */
  static bool seeingFinishLine(const std::vector<FVector2> &vectors) {
    return (numberOfFinishLineVectors(vectors) >=
            Params::FINISH_LINE_NUMBER_OF_VECTORS_THSH);
  }

  static const FVector2 ZERO;  ///< Vectorul nul (0, 0).
  static const FVector2 NORTH; ///< Vectorul nord (0, 1).
};

} // namespace ls
