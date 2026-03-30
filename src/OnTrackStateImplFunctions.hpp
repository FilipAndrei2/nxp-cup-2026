#pragma once
#include "math/Vector2.hpp"
#include "math/Vectors.hpp"
#include "utils/lifesource.hpp"
#include <array>
#include <vector>

namespace ls {
static uint8_t filterTrackVectors(const std::vector<ls::FVector2> &vectors,
                                  std::array<ls::FVector2, 4> &outInfoVectors);

} // namespace ls
