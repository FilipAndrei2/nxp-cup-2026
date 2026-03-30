#pragma once
#include "math/Vector2.hpp"
#include "utils/lifesource.hpp"
#include <array>
#include <vector>
namespace ls {
static uint8_t filterInfoVectors(const std::vector<FVector2> &inVectors,
                                 std::array<FVector2, 5> &outInfoVectors);

static void computeSpeedAndAngle(const std::array<FVector2, 5> &inVectors,
                                 const uint8_t inNumberInfoVectors,
                                 angle_t &outAngle, speed_t &outSpeed);
} // namespace ls