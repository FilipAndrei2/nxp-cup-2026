/**
 * @file Vectors.cpp
 * @brief Definițiile constantelor statice ale clasei @c Vectors.
 */
#include "math/Vectors.hpp"

namespace ls {

/// @brief Vectorul nul (0, 0).
const FVector2 Vectors::ZERO(0.0f, 0.0f);

/// @brief Vectorul nord (0, 1) — direcția de referință pentru calcule de unghi.
const FVector2 Vectors::NORTH(0.0f, 1.0f);

} // namespace ls
