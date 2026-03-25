#pragma once
#include "utils/lifesource.hpp"

namespace ls {
struct FVector2;
static uint8_t filterTrackVectors(const std::vector<ls::FVector2> &vectors,
                                  std::array<ls::FVector2, 4> &outInfoVectors);

static void computeSpeedAndAngle(const std::array<FVector2, 4> &inInfoVectors,
                                 uint8_t inNumberInfoVectors, angle_t &outAngle,
                                 speed_t &outSpeed);
} // namespace ls
