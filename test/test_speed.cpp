/* test/test_speed.cpp
 * Tests for ls::Speed – speed constants and both scaling overloads.
 *
 * Speed::scale(speed_t, angle_t)
 *   Formula: (speed_t)((angle_t)maxSpeed * std::cos(angle))
 *   Range: [-maxSpeed, maxSpeed], but angle inputs are usually [0, PI/2].
 *
 * Speed::scale(speed_t, proximity_t)
 *   Formula: static_cast<speed_t>(maxSpeed * (cubeProxi / 100.0f))
 *   Uses floating-point division so intermediate values 1-99 produce
 *   proportional results; proximity=50 → 50% of maxSpeed, etc.
 */
#include "catch2/catch_amalgamated.hpp"
#include "params/speed.hpp"

#include <cmath>

using namespace ls;
using Catch::Approx;

static constexpr float PI = 3.14159265358979323846f;

// ── Constants ─────────────────────────────────────────────────────────────────

TEST_CASE("Speed::MAX constant is 100", "[speed]") {
    CHECK(Speed::MAX == 100);
}

TEST_CASE("Speed::WAITING_CUBE_SPEED constant is 30", "[speed]") {
    CHECK(Speed::WAITING_CUBE_SPEED == 30);
}

TEST_CASE("Speed::_4_WAY_CROSSWAY_SPEED constant is 40", "[speed]") {
    CHECK(Speed::_4_WAY_CROSSWAY_SPEED == 40);
}

// ── Speed::scale(speed_t, angle_t) ───────────────────────────────────────────

TEST_CASE("Speed::scale(speed, angle) – zero angle preserves maxSpeed", "[speed]") {
    // cos(0) = 1 → no reduction
    CHECK(Speed::scale(Speed::MAX, 0.0f) == Speed::MAX);
    CHECK(Speed::scale(Speed::WAITING_CUBE_SPEED, 0.0f) == Speed::WAITING_CUBE_SPEED);
    CHECK(Speed::scale(Speed::_4_WAY_CROSSWAY_SPEED, 0.0f) == Speed::_4_WAY_CROSSWAY_SPEED);
}

TEST_CASE("Speed::scale(speed, angle) – PI/2 angle gives 0", "[speed]") {
    // cos(PI/2) ≈ 6.12e-17 → truncates to 0 after cast to speed_t (int32_t)
    CHECK(Speed::scale(Speed::MAX, PI / 2.0f) == 0);
    CHECK(Speed::scale(Speed::WAITING_CUBE_SPEED, PI / 2.0f) == 0);
}

TEST_CASE("Speed::scale(speed, angle) – PI/4 angle scales by cos(PI/4)", "[speed]") {
    // cos(PI/4) ≈ 0.7071 → for MAX=100: (int)(100 * 0.7071) = 70
    int expected = static_cast<int>(static_cast<float>(Speed::MAX) * std::cos(PI / 4.0f));
    CHECK(Speed::scale(Speed::MAX, PI / 4.0f) == expected);
}

TEST_CASE("Speed::scale(speed, angle) – PI/6 angle scales by cos(PI/6)", "[speed]") {
    // cos(PI/6) ≈ 0.866 → for MAX=100: (int)(100 * 0.866) = 86
    int expected = static_cast<int>(static_cast<float>(Speed::MAX) * std::cos(PI / 6.0f));
    CHECK(Speed::scale(Speed::MAX, PI / 6.0f) == expected);
}

TEST_CASE("Speed::scale(speed, angle) – symmetry: negative angles give same result", "[speed]") {
    // cos is even, so cos(-a) == cos(a)
    CHECK(Speed::scale(100, PI / 6.0f) == Speed::scale(100, -PI / 6.0f));
    CHECK(Speed::scale(100, PI / 4.0f) == Speed::scale(100, -PI / 4.0f));
    CHECK(Speed::scale(100, PI / 3.0f) == Speed::scale(100, -PI / 3.0f));
}

TEST_CASE("Speed::scale(speed, angle) – zero maxSpeed always gives 0", "[speed]") {
    CHECK(Speed::scale(0, 0.0f) == 0);
    CHECK(Speed::scale(0, PI / 4.0f) == 0);
    CHECK(Speed::scale(0, PI / 2.0f) == 0);
}

TEST_CASE("Speed::scale(speed, angle) – WAITING_CUBE_SPEED at PI/4", "[speed]") {
    int expected = static_cast<int>(static_cast<float>(Speed::WAITING_CUBE_SPEED)
                                    * std::cos(PI / 4.0f));
    CHECK(Speed::scale(Speed::WAITING_CUBE_SPEED, PI / 4.0f) == expected);
}

// ── Speed::scale(speed_t, proximity_t) ───────────────────────────────────────
// Uses floating-point division: static_cast<speed_t>(maxSpeed * (cubeProxi / 100.0f))

TEST_CASE("Speed::scale(speed, proximity) – proximity 0 gives 0", "[speed]") {
    CHECK(Speed::scale(Speed::MAX, static_cast<proximity_t>(0)) == 0);
    CHECK(Speed::scale(Speed::WAITING_CUBE_SPEED, static_cast<proximity_t>(0)) == 0);
}

TEST_CASE("Speed::scale(speed, proximity) – proximity 100 preserves maxSpeed", "[speed]") {
    // 100 / 100.0 = 1.0
    CHECK(Speed::scale(Speed::MAX, static_cast<proximity_t>(100)) == Speed::MAX);
    CHECK(Speed::scale(Speed::WAITING_CUBE_SPEED, static_cast<proximity_t>(100))
          == Speed::WAITING_CUBE_SPEED);
}

TEST_CASE("Speed::scale(speed, proximity) – intermediate proximities scale proportionally", "[speed]") {
    // Float division: proximity/100.0 gives a proper fraction
    CHECK(Speed::scale(100, static_cast<proximity_t>(50)) == 50);
    CHECK(Speed::scale(100, static_cast<proximity_t>(25)) == 25);
    CHECK(Speed::scale(100, static_cast<proximity_t>(75)) == 75);
    CHECK(Speed::scale(30,  static_cast<proximity_t>(50)) == 15);
}

TEST_CASE("Speed::scale(speed, proximity) – proximity 1 gives a non-zero result", "[speed]") {
    // With float division, proximity=1 → 100 * (1/100.0) = 1 (rounded down via cast)
    CHECK(Speed::scale(100, static_cast<proximity_t>(1)) == 1);
}

TEST_CASE("Speed::scale(speed, proximity) – zero maxSpeed with any proximity gives 0", "[speed]") {
    CHECK(Speed::scale(0, static_cast<proximity_t>(0))   == 0);
    CHECK(Speed::scale(0, static_cast<proximity_t>(100)) == 0);
    CHECK(Speed::scale(0, static_cast<proximity_t>(50))  == 0);
}
