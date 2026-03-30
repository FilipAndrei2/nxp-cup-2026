/* test/test_bugs.cpp
 *
 * Tests that capture INTENDED behaviour for each documented bug.
 * Every test in this file is expected to FAIL against the current (buggy)
 * implementation.  When a bug is fixed the corresponding test(s) will pass.
 *
 * Bug references match the IDs in BUGS.md.
 */

#include "catch2/catch_amalgamated.hpp"

#include "dto/DrivingCommandDTO.hpp"
#include "dto/SensorDataDTO.hpp"
#include "math/Floats.hpp"
#include "math/Vector2.hpp"
#include "math/Vectors.hpp"
#include "params/speed.hpp"
#include "track_states/OnTrackState.hpp"

#include <cmath>
#include <memory>
#include <vector>

using namespace ls;
using Catch::Approx;

// ── Helper: keep shared_ptr and SensorDataDTO alive together ─────────────────

struct SensorFixture {
    std::shared_ptr<std::vector<FVector2>> sp;
    SensorDataDTO dto;

    SensorFixture(std::vector<FVector2> vecs, proximity_t cube = 0)
        : sp(std::make_shared<std::vector<FVector2>>(std::move(vecs)))
        , dto{sp, cube}
    {}
};

// =============================================================================
// BUG-1 · CRITICAL
// src/OnTrackState.cpp – wrong angle formula in the 3+ vectors (default) case.
//
// The algorithm comment says:
//   "Ii luam doar pe primi 2 si aplicam acelasi algoritm ca in cazul cu 2 vectori"
//   ("take only the first 2 and apply the same algorithm as the 2-vector case")
//
// The 2-vector case computes:
//   medi  = Avg(v0, v1)
//   angle = AngleBetween(medi, NORTH)
//
// The default case computes medi but then calls
//   angle = AngleBetween(v0, v1)          ← BUG: ignores medi and NORTH
// instead of
//   angle = AngleBetween(medi, NORTH)     ← intended
// =============================================================================

TEST_CASE("BUG-1: OnTrackState 3+ vectors – angle is avg-vs-NORTH, not raw-vs-raw",
          "[bug][bug1][ontrack]") {
    // Two track vectors symmetric around NORTH:
    //   v0 = ( 1, 1)  →  45° east of NORTH
    //   v1 = (-1, 1)  →  45° west of NORTH
    // Their average is (0, 1) = NORTH, so the intended angle is 0 and speed is MAX.
    //
    // The buggy code computes AngleBetween(v0, v1):
    //   dot(v0,v1) = (1)(-1)+(1)(1) = 0  →  acos(0 / (√2·√2)) = π/2
    // giving angle ≈ π/2 and speed ≈ 0 (cos(π/2) ≈ 0).
    //
    // A third vector is added so the switch reaches the `default` branch (3+).
    FVector2 v0( 1.0f,  1.0f); // 45° east of NORTH
    FVector2 v1(-1.0f,  1.0f); // 45° west of NORTH
    FVector2 v2( 0.0f,  1.0f); // NORTH – a third non-finish, non-zero vector

    auto &state = OnTrackState::getInstance();
    SensorFixture sf({v0, v1, v2});
    auto cmd = state.computeCommand(sf.dto);

    // Intended: avg of v0 and v1 points NORTH → angle ≈ 0 → speed ≈ MAX
    // Buggy:    angle ≈ π/2, speed ≈ 0
    CHECK(cmd.angle == Approx(0.0f).margin(0.01f));
    CHECK(cmd.speed == Speed::MAX);
}

TEST_CASE("BUG-1: OnTrackState default case – result matches case-2 formula",
          "[bug][bug1][ontrack]") {
    // When 3+ vectors are present the algorithm says it "applies the same algorithm
    // as the 2-vector case".  So the output must be identical to the output of the
    // 2-vector case for the same pair of lead vectors.

    FVector2 v0(0.5f, 1.0f);
    FVector2 v1(0.3f, 0.9f);
    FVector2 v2(0.1f, 0.8f); // extra vector to trigger the default branch

    auto &state = OnTrackState::getInstance();

    // 2-vector result (case 2 branch)
    SensorFixture sf2({v0, v1});
    auto cmd2 = state.computeCommand(sf2.dto);

    // 3-vector result (default branch) – must equal the 2-vector result
    SensorFixture sf3({v0, v1, v2});
    auto cmd3 = state.computeCommand(sf3.dto);

    CHECK(cmd3.angle == Approx(cmd2.angle).margin(0.001f));
    CHECK(cmd3.speed == cmd2.speed);
}

// =============================================================================
// BUG-2 · HIGH
// include/params/speed.hpp – Speed::scale(speed_t, proximity_t) uses integer
// division so proximity values 1–99 all produce 0.
//
// Intended: linear scaling – proximity 50 → 50 % of maxSpeed.
// Buggy:    proximity 50 → maxSpeed * (50/100) = maxSpeed * 0 = 0.
// =============================================================================

TEST_CASE("BUG-2: Speed::scale(speed, proximity) – proximity 50 gives ~50% of maxSpeed",
          "[bug][bug2][speed]") {
    // Intended: proportional scaling.
    // 50 % of MAX (100) should be 50.
    // The buggy implementation returns 100 * (50/100) = 100 * 0 = 0.
    speed_t result = Speed::scale(Speed::MAX, static_cast<proximity_t>(50));
    CHECK(result > 0);
    CHECK(result == 50);
}

TEST_CASE("BUG-2: Speed::scale(speed, proximity) – proximity 1 gives non-zero speed",
          "[bug][bug2][speed]") {
    // Even the smallest non-zero proximity should yield a non-zero (albeit very
    // small) speed.  Integer division makes this return 0.
    speed_t result = Speed::scale(Speed::MAX, static_cast<proximity_t>(1));
    CHECK(result > 0);
}

TEST_CASE("BUG-2: Speed::scale(speed, proximity) – proximity 75 gives ~75% of WAITING_CUBE_SPEED",
          "[bug][bug2][speed]") {
    speed_t maxSpd  = Speed::WAITING_CUBE_SPEED; // 30
    speed_t result  = Speed::scale(maxSpd, static_cast<proximity_t>(75));
    speed_t expected = static_cast<speed_t>(maxSpd * 0.75f); // 22
    CHECK(result == expected);
}

// =============================================================================
// BUG-3 · HIGH
// include/math/Vector2.hpp – the 4-argument constructor Vector2(x1,y1,x2,y2)
// has an empty body; x and y are left at 0 instead of being set to (x2-x1, y2-y1).
// =============================================================================

TEST_CASE("BUG-3: Vector2 4-arg constructor creates direction vector (x2-x1, y2-y1)",
          "[bug][bug3][vector2]") {
    // Construct a vector from point (1, 2) to point (4, 6).
    // Expected direction: (4-1, 6-2) = (3, 4).
    // Buggy result: (0, 0).
    FVector2 v(1.0f, 2.0f, 4.0f, 6.0f);
    CHECK(v.getX() == Approx(3.0f));
    CHECK(v.getY() == Approx(4.0f));
}

TEST_CASE("BUG-3: Vector2 4-arg constructor – origin-to-point is same as 2-arg constructor",
          "[bug][bug3][vector2]") {
    // Vector2(0,0,x,y) should equal Vector2(x,y).
    FVector2 fromEndpoints(0.0f, 0.0f, 5.0f, 3.0f);
    FVector2 direct(5.0f, 3.0f);
    CHECK(fromEndpoints.getX() == Approx(direct.getX()));
    CHECK(fromEndpoints.getY() == Approx(direct.getY()));
}

TEST_CASE("BUG-3: Vector2 4-arg constructor – reversed direction negates components",
          "[bug][bug3][vector2]") {
    // From (3, 4) to (1, 2): direction = (-2, -2).
    FVector2 v(3.0f, 4.0f, 1.0f, 2.0f);
    CHECK(v.getX() == Approx(-2.0f));
    CHECK(v.getY() == Approx(-2.0f));
}

// =============================================================================
// BUG-4 · HIGH
// include/math/Vector2.hpp – Vector2::AngleBetween divides by zero when either
// input vector has length 0, producing NaN.
//
// Intended: return a well-defined finite value (e.g. 0) for the zero vector.
// Buggy:    returns NaN.
// =============================================================================

TEST_CASE("BUG-4: AngleBetween – zero vector does not produce NaN",
          "[bug][bug4][vector2]") {
    FVector2 zero(0.0f, 0.0f);
    FVector2 north(0.0f, 1.0f);

    float angle = Vector2<float>::AngleBetween(zero, north);

    // NaN is not equal to itself – that is the standard way to detect NaN.
    CHECK(angle == angle); // fails if angle is NaN
    CHECK(std::isfinite(angle));
}

TEST_CASE("BUG-4: AngleBetween – two zero vectors do not produce NaN",
          "[bug][bug4][vector2]") {
    FVector2 zero(0.0f, 0.0f);

    float angle = Vector2<float>::AngleBetween(zero, zero);

    CHECK(angle == angle); // fails if angle is NaN
    CHECK(std::isfinite(angle));
}

// =============================================================================
// BUG-5 · MEDIUM
// include/math/Vector2.hpp – Vector2::normalized() divides by zero for a
// zero-length vector, producing NaN components.
//
// Intended: return a well-defined vector (e.g. the zero vector itself).
// Buggy:    returns (NaN, NaN).
// =============================================================================

TEST_CASE("BUG-5: normalized() on zero vector does not produce NaN components",
          "[bug][bug5][vector2]") {
    FVector2 zero(0.0f, 0.0f);
    FVector2 n = zero.normalized();

    CHECK(n.getX() == n.getX()); // fails if NaN
    CHECK(n.getY() == n.getY()); // fails if NaN
    CHECK(std::isfinite(n.getX()));
    CHECK(std::isfinite(n.getY()));
}
