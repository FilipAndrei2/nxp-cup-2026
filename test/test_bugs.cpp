/* test/test_bugs.cpp
 * Failing regression tests that expose known bugs in the codebase.
 *
 * Each test expresses the INTENDED behaviour of the API under test.
 * Every test in this file is expected to FAIL until the corresponding
 * bug is fixed.  Tests are tagged [bugN] so they can be run in isolation:
 *
 *   ./run_tests [bug1]   # normalize/normalized on zero vector
 *   ./run_tests [bug2]   # unclamped acos → NaN
 *   ./run_tests [bug3]   # int32_t zero-guard wrong threshold
 *   ./run_tests [bug4]   # SeeingFirstFinishState skips detectCube()
 *   ./run_tests [bug5]   # setState(nullptr) null-ptr dereference (SKIPPED)
 *   ./run_tests [bug6]   # 4-arg constructor always gives x ≥ 0
 *   ./run_tests [bug7]   # OnTrackState ignores inCubeProxi parameter
 *   ./run_tests [bug9]   # computeAngle ignores vectors [2..N-1]
 *   ./run_tests [bug10]  # uint8_t finish-line counter overflows at 256
 *   ./run_tests [bug11]  # Speed::scale doesn't clamp proximity > 100
 */
#include "catch2/catch_amalgamated.hpp"

#include "dto/DrivingCommandDTO.hpp"
#include "dto/SensorDataDTO.hpp"
#include "math/Vector2.hpp"
#include "math/Vectors.hpp"
#include "params/Params.hpp"
#include "params/speed.hpp"
#include "track_states/ATrackStateContext.hpp"
#include "track_states/BreakingState.hpp"
#include "track_states/ITrackState.hpp"
#include "track_states/OnTrackState.hpp"
#include "track_states/SeeingFirstFinishState.hpp"
#include "track_states/WaitingCubeState.hpp"

#include <cmath>
#include <memory>
#include <vector>

using namespace ls;
using Catch::Approx;

static constexpr float PI = 3.14159265358979323846f;

// ── Test helpers ──────────────────────────────────────────────────────────────

class TestableContext : public ATrackStateContext {
public:
    TestableContext() = default;
    ITrackState *currentState() { return &getState(); }
};

struct SensorFixture {
    std::shared_ptr<std::vector<FVector2>> sp;
    SensorDataDTO dto;

    SensorFixture(std::vector<FVector2> vecs, proximity_t cube = 0)
        : sp(std::make_shared<std::vector<FVector2>>(std::move(vecs)))
        , dto{sp, cube}
    {}
};

// ═════════════════════════════════════════════════════════════════════════════
//  Bug 1 – Vector2::normalize() / normalized() divide-by-zero on zero vector
//
//  Root cause: both methods divide x and y by len() without checking whether
//  len() == 0.  For a (0, 0) vector, x /= 0.0f and y /= 0.0f produce NaN.
//
//  Intended: normalize() is a no-op on the zero vector (or clamps to zero);
//            normalized() returns (0, 0); neither produces NaN.
//  Actual:   both produce NaN in both components.
// ═════════════════════════════════════════════════════════════════════════════

TEST_CASE("Bug 1a – normalize() on zero vector must not produce NaN",
          "[vector2][bug1]") {
    FVector2 v(0.0f, 0.0f);
    v.normalize();
    CHECK_FALSE(std::isnan(v.getX()));
    CHECK_FALSE(std::isnan(v.getY()));
}

TEST_CASE("Bug 1b – normalized() on zero vector must not produce NaN",
          "[vector2][bug1]") {
    FVector2 n = FVector2(0.0f, 0.0f).normalized();
    CHECK_FALSE(std::isnan(n.getX()));
    CHECK_FALSE(std::isnan(n.getY()));
}

TEST_CASE("Bug 1c – normalized() on zero vector should return the zero vector",
          "[vector2][bug1]") {
    FVector2 n = FVector2(0.0f, 0.0f).normalized();
    CHECK(n.getX() == Approx(0.0f));
    CHECK(n.getY() == Approx(0.0f));
}

TEST_CASE("Bug 1d – normalize() on zero vector should leave it at (0, 0)",
          "[vector2][bug1]") {
    FVector2 v(0.0f, 0.0f);
    v.normalize();
    CHECK(v.getX() == Approx(0.0f));
    CHECK(v.getY() == Approx(0.0f));
}

// ═════════════════════════════════════════════════════════════════════════════
//  Bug 2 – AngleBetween: std::acos called without clamping → NaN
//
//  Root cause: when both vectors pass isNormalized() the code does
//    acos(Dot(lhs, rhs))
//  without clamping the dot product to [-1, 1].  Floating-point rounding
//  causes Dot(v, v) to slightly exceed 1.0 for a vector with len = 1 + ε
//  that still satisfies isNormalized() (Floats::eq tolerance is 1e-6).
//
//  Intended: angle is a finite value in [0, PI].
//  Actual:   std::acos receives an argument > 1.0 → returns NaN.
// ═════════════════════════════════════════════════════════════════════════════

TEST_CASE("Bug 2a – AngleBetween: nearly-normalised vector must not produce NaN",
          "[vector2][bug2]") {
    // len = 1 + 4e-7 → |len - 1| = 4e-7 < FLOAT_EPS (1e-6) → isNormalized() == true
    // Dot(v, v) = (1 + 4e-7)^2 ≈ 1 + 8e-7 > 1.0 → unclamped acos → NaN
    FVector2 a(0.0f, 1.0f + 4e-7f);
    REQUIRE(a.isNormalized()); // confirm the precondition holds

    float angle = FVector2::AngleBetween(a, FVector2::NORTH);
    CHECK_FALSE(std::isnan(angle));
}

TEST_CASE("Bug 2b – AngleBetween: result for nearly-normalised vectors must be in [0, PI]",
          "[vector2][bug2]") {
    FVector2 a(0.0f, 1.0f + 4e-7f);
    float angle = FVector2::AngleBetween(a, FVector2::NORTH);
    CHECK(angle >= 0.0f);
    CHECK(angle <= PI);
}

TEST_CASE("Bug 2c – AngleBetween: a vector dot-producted with itself must give a finite angle",
          "[vector2][bug2]") {
    // Any unit-length (or near-unit) vector dot-producted with itself should give 0.
    // With clamping the result is 0.0 rad; without clamping it may be NaN.
    FVector2 a(0.0f, 1.0f + 4e-7f);
    float angle = FVector2::AngleBetween(a, a);
    CHECK_FALSE(std::isnan(angle));
    CHECK(angle == Approx(0.0f).margin(1e-4f));
}

// ═════════════════════════════════════════════════════════════════════════════
//  Bug 3 – AngleBetween zero-guard uses int32_t cast: wrong threshold
//
//  Root cause:
//    if ((int32_t)(llen * 1000) == 0 || ...) { return 0.0f; }
//  The cast truncates to int, so any len in (0, 0.001) evaluates to 0 and
//  incorrectly returns 0.0f even though the vector has a valid direction.
//  A proper epsilon guard (e.g. len < 1e-6f) should only suppress truly
//  zero-ish vectors.
//
//  Intended: AngleBetween((0.0008, 0), NORTH) ≈ PI/2.
//  Actual:   (int32_t)(0.0008 * 1000) = 0 → guard fires → returns 0.0f.
// ═════════════════════════════════════════════════════════════════════════════

TEST_CASE("Bug 3a – AngleBetween: small-but-valid East vector (len≈0.0008) must give PI/2 from NORTH",
          "[vector2][bug3]") {
    // len = 0.0008 → (int32_t)(0.8) = 0 → zero-guard fires → 0.0f returned
    FVector2 east(0.0008f, 0.0f);
    float angle = FVector2::AngleBetween(east, FVector2::NORTH);
    CHECK(angle == Approx(PI / 2.0f).epsilon(1e-3f));
}

TEST_CASE("Bug 3b – AngleBetween: small-but-valid East vector (len≈0.0005) must give PI/2 from NORTH",
          "[vector2][bug3]") {
    // len = 0.0005 → (int32_t)(0.5) = 0 → zero-guard fires → 0.0f returned
    FVector2 east(0.0005f, 0.0f);
    float angle = FVector2::AngleBetween(east, FVector2::NORTH);
    CHECK(angle == Approx(PI / 2.0f).epsilon(1e-2f));
}

TEST_CASE("Bug 3c – AngleBetween: zero-guard must not suppress vectors with len > 1e-6",
          "[vector2][bug3]") {
    // The guard exists to prevent division-by-zero.  Only truly zero-length
    // vectors (len ≈ 0) should trigger it.  A vector of len 0.0008 is not zero.
    FVector2 small_north(0.0f, 0.0009f); // points North, tiny magnitude
    float angle = FVector2::AngleBetween(small_north, FVector2::NORTH);
    // Angle between a tiny-North and NORTH should be 0, not guarded-zero
    // (both are 0 here, but the path taken matters: it must compute, not guard)
    CHECK_FALSE(std::isnan(angle));
    CHECK(angle == Approx(0.0f).margin(1e-3f));
}

// ═════════════════════════════════════════════════════════════════════════════
//  Bug 4 – SeeingFirstFinishState::computeCommand skips detectCube()
//
//  Root cause: the overridden computeCommand calls filterVectors() and
//  computeAngle/computeSpeed but omits detectCube(sensorData.cubeProximity).
//  The static cubeDetected/cubeProximity fields are therefore never updated
//  while the state machine is in SeeingFirstFinishState.
//
//  Consequence: a stale cubeDetected=true (set by a preceding state) causes
//  WaitingCubeState to incorrectly transition to BreakingState even though
//  the current sensorData reports proximity = 0 (no cube present).
//
//  Intended: after SeeingFirstFinishState processes sensorData with proximity=0,
//            cubeDetected is false → WaitingCubeState stays in WaitingCubeState.
//  Actual:   cubeDetected remains true → WaitingCubeState transitions to BreakingState.
// ═════════════════════════════════════════════════════════════════════════════

TEST_CASE("Bug 4 – SeeingFirstFinishState must call detectCube() to prevent stale cubeDetected",
          "[states][bug4]") {
    // Step 1: force cubeDetected = true by running OnTrackState with high proximity.
    {
        SensorFixture sf({FVector2(0.0f, 1.0f)}, /*cubeProx=*/100);
        OnTrackState::getInstance().computeCommand(sf.dto);
    }

    // Step 2: SeeingFirstFinishState runs with proximity = 0.
    //         Intended: detectCube(0) resets cubeDetected to false.
    //         Bug:      detectCube() is never called; cubeDetected stays true.
    {
        SensorFixture sf({FVector2(1.0f, 0.0f)}, /*cubeProx=*/0);
        SeeingFirstFinishState::getInstance().computeCommand(sf.dto);
    }

    // Step 3: WaitingCubeState must NOT transition when proximity = 0.
    TestableContext ctx;
    ctx.setState(&WaitingCubeState::getInstance());
    WaitingCubeState::getInstance().updateNextState(ctx);

    // Intended: cubeDetected == false → state unchanged.
    // Bug:      cubeDetected is still true → transitions to BreakingState.
    CHECK(ctx.currentState() == &WaitingCubeState::getInstance());
}

// ═════════════════════════════════════════════════════════════════════════════
//  Bug 5 – ATrackStateContext::setState(nullptr) causes null-ptr dereference
//
//  Root cause: setState stores the pointer without a null check; getState()
//  immediately dereferences it with `return *state`.  Calling setState(nullptr)
//  followed by any state access is undefined behaviour / crash.
//
//  This test is SKIPPED because exercising the bug would abort the test runner
//  with a segmentation fault rather than a normal assertion failure.  The test
//  documents the intended behaviour that a null-guard should enforce.
// ═════════════════════════════════════════════════════════════════════════════

TEST_CASE("Bug 5 – setState(nullptr) must be a no-op (SKIPPED: UB/crash before fix)",
          "[states][bug5]") {
    SKIP("Bug 5: setState(nullptr) stores null; the subsequent getState() dereferences "
         "nullptr → undefined behaviour (segfault). "
         "After the fix, setState(nullptr) must leave the current state unchanged. "
         "Remove this SKIP and uncomment the assertions below once the null-guard is added.");

    // After fixing Bug 5 these assertions must hold:
    //   TestableContext ctx;
    //   ITrackState *before = ctx.currentState();
    //   ctx.setState(nullptr);
    //   CHECK(ctx.currentState() == before);
}

// ═════════════════════════════════════════════════════════════════════════════
//  Bug 6 – 4-argument Vector2 constructor always produces x ≥ 0
//
//  Root cause:
//    this->x = x0 > x1 ? x0 - x1 : x1 - x0;  // always |x0 - x1|
//  The correct formula (matching PixyCamControllerImpl) is:
//    x = (y0 > y1) ? (x1 - x0) : (x0 - x1)   // signed, indicates direction
//
//  Intended: two segment endpoints with opposite horizontal orientations
//            produce x values with opposite signs.
//  Actual:   both always give a positive |Δx|.
// ═════════════════════════════════════════════════════════════════════════════

TEST_CASE("Bug 6a – 4-arg constructor: opposing horizontal segments must give opposite-sign x",
          "[vector2][bug6]") {
    // Segment A: (x0=3, y0=1) → (x1=5, y1=0)
    //   y0 > y1 → x = x1 - x0 = 5 - 3 = +2  (going right)
    FVector2 vecA(3.0f, 1.0f, 5.0f, 0.0f);

    // Segment B: (x0=5, y0=1) → (x1=3, y1=0)
    //   y0 > y1 → x = x1 - x0 = 3 - 5 = -2  (going left)
    FVector2 vecB(5.0f, 1.0f, 3.0f, 0.0f);

    // Intended: vecA.x > 0, vecB.x < 0, and vecA.x == -vecB.x
    // Bug:      both give x = |Δx| = 2 (same sign, cannot distinguish direction)
    CHECK(vecA.getX() == Approx(-vecB.getX()));
}

TEST_CASE("Bug 6b – 4-arg constructor: leftward segment must have negative x",
          "[vector2][bug6]") {
    // (x0=5, y0=1) → (x1=3, y1=0): y0 > y1, direction is left → x = x1 - x0 = -2
    FVector2 v(5.0f, 1.0f, 3.0f, 0.0f);
    CHECK(v.getX() < 0.0f);
}

TEST_CASE("Bug 6c – 4-arg constructor: rightward segment must have positive x",
          "[vector2][bug6]") {
    // (x0=3, y0=1) → (x1=5, y1=0): y0 > y1, direction is right → x = x1 - x0 = +2
    FVector2 v(3.0f, 1.0f, 5.0f, 0.0f);
    CHECK(v.getX() > 0.0f);
}

// ═════════════════════════════════════════════════════════════════════════════
//  Bug 7 – OnTrackState::computeSpeed ignores inCubeProxi parameter
//
//  Root cause: computeSpeed always returns Speed::scale(MAX, inAngle) and
//  never reads inCubeProxi, so the car travels at full speed even when a
//  cube is approaching on the main track.
//
//  Intended: speed decreases as cube proximity increases (inCubeProxi > 0).
//  Actual:   speed is the same regardless of proximity.
// ═════════════════════════════════════════════════════════════════════════════

TEST_CASE("Bug 7 – OnTrackState speed must decrease as cube proximity increases",
          "[states][bug7]") {
    auto &state = OnTrackState::getInstance();

    // Straight-ahead NORTH vector → angle ≈ 0 → cos(0) = 1, so angle-based
    // scaling keeps speed at MAX.  Only cube-proximity scaling should reduce it.
    SensorFixture sf_far ({FVector2(0.0f, 1.0f)}, /*proximity=*/0);
    SensorFixture sf_near({FVector2(0.0f, 1.0f)}, /*proximity=*/80);

    auto cmd_far  = state.computeCommand(sf_far.dto);
    auto cmd_near = state.computeCommand(sf_near.dto);

    // Intended: approaching cube slows the car.
    // Bug:      both commands carry the same speed (MAX * cos(0) = MAX).
    CHECK(cmd_near.speed < cmd_far.speed);
}

// ═════════════════════════════════════════════════════════════════════════════
//  Bug 9 – ATrackState::computeAngle default case ignores vectors [2..N-1]
//
//  Root cause: the switch default branch does the same as case 2 — it only
//  averages filteredVectors[0] and [1], silently discarding [2] onwards.
//
//  Intended: all N filtered vectors contribute to the computed angle.
//  Actual:   vectors beyond index 1 are ignored, so the angle is biased
//            toward whatever the first two vectors indicate.
// ═════════════════════════════════════════════════════════════════════════════

TEST_CASE("Bug 9 – computeAngle with 3 vectors must differ from using only the first two",
          "[states][bug9]") {
    // v0 and v1 both lean strongly to the right (+x direction).
    // v2 leans strongly to the left (−x), with enough magnitude to swing the
    // mean leftward when all three vectors are properly included.
    FVector2 v0( 1.0f, 0.5f);  // rightward
    FVector2 v1( 1.0f, 0.5f);  // rightward
    FVector2 v2(-3.0f, 0.5f);  // strongly leftward (|x| > combined |x| of v0+v1)

    // Compute the angle that the buggy implementation produces:
    // it only averages v0 and v1 → avg = (1.0, 0.5), angle ≈ 1.107 rad.
    FVector2 avg_only_01 = FVector2::Avg(v0, v1);
    float bug_angle = FVector2::AngleBetween(avg_only_01, Vectors::NORTH);

    auto &state = OnTrackState::getInstance();
    SensorFixture sf({v0, v1, v2});
    auto cmd = state.computeCommand(sf.dto);

    // Intended: v2 is included in the computation → angle differs from bug_angle.
    // Bug:      cmd.angle == bug_angle (v2 is ignored).
    CHECK(cmd.angle != Approx(bug_angle).epsilon(1e-3f));
}

TEST_CASE("Bug 9b – computeAngle: 4 vectors must not give same result as 2 vectors",
          "[states][bug9]") {
    // Two identical rightward vectors plus two strongly leftward vectors.
    // A proper 4-vector average should lean left; the buggy impl always uses
    // only [0] and [1], giving a rightward result.
    FVector2 v0( 1.0f, 0.2f);
    FVector2 v1( 1.0f, 0.2f);
    FVector2 v2(-2.0f, 0.2f);
    FVector2 v3(-2.0f, 0.2f);

    FVector2 avg_only_01 = FVector2::Avg(v0, v1);
    float bug_angle = FVector2::AngleBetween(avg_only_01, Vectors::NORTH);

    auto &state = OnTrackState::getInstance();
    SensorFixture sf({v0, v1, v2, v3});
    auto cmd = state.computeCommand(sf.dto);

    CHECK(cmd.angle != Approx(bug_angle).epsilon(1e-3f));
}

// ═════════════════════════════════════════════════════════════════════════════
//  Bug 10 – uint8_t finish-line vector counter overflows at 256
//
//  Root cause: numberOfFinishLineVectors uses a uint8_t counter.  Adding 256
//  finish-line vectors wraps it back to 0, causing seeingFinishLine() to
//  return false even though 256 horizontal vectors are present.
//
//  Intended: count > 0; seeingFinishLine() == true.
//  Actual:   count == 0 (wrapped); seeingFinishLine() == false.
// ═════════════════════════════════════════════════════════════════════════════

TEST_CASE("Bug 10a – numberOfFinishLineVectors must not overflow to 0 for 256 vectors",
          "[vectors][bug10]") {
    std::vector<FVector2> vecs;
    for (int i = 0; i < 256; ++i) {
        vecs.emplace_back(1.0f, 0.0f); // perfectly horizontal → finish-line
    }
    auto count = Vectors::numberOfFinishLineVectors(vecs);
    CHECK(count > 0);
}

TEST_CASE("Bug 10b – seeingFinishLine must return true for 256 finish-line vectors",
          "[vectors][bug10]") {
    std::vector<FVector2> vecs;
    for (int i = 0; i < 256; ++i) {
        vecs.emplace_back(1.0f, 0.0f);
    }
    CHECK(Vectors::seeingFinishLine(vecs));
}

TEST_CASE("Bug 10c – numberOfFinishLineVectors must not overflow to 0 for 512 vectors",
          "[vectors][bug10]") {
    std::vector<FVector2> vecs;
    for (int i = 0; i < 512; ++i) {
        vecs.emplace_back(1.0f, 0.0f);
    }
    // 512 % 256 == 0 → double-overflow, same symptom
    auto count = Vectors::numberOfFinishLineVectors(vecs);
    CHECK(count > 0);
}

// ═════════════════════════════════════════════════════════════════════════════
//  Bug 11 – Speed::scale(speed, proximity) does not clamp proximity to [0, 100]
//
//  Root cause:
//    return (speed_t)((float)maxSpeed * ((float)cubeProxi / 100.0f));
//  When cubeProxi > 100 the result exceeds maxSpeed, violating the
//  documented contract that proximity is a percentage in [0, 100].
//
//  Intended: result ≤ maxSpeed for any proximity value.
//  Actual:   result = maxSpeed * (proximity / 100), which exceeds maxSpeed
//            when proximity > 100.
// ═════════════════════════════════════════════════════════════════════════════

TEST_CASE("Bug 11a – Speed::scale(maxSpeed, 150) must not exceed maxSpeed",
          "[speed][bug11]") {
    // proximity = 150 → current: 100 * 1.5 = 150 > Speed::MAX (100)
    speed_t result = Speed::scale(Speed::MAX, static_cast<proximity_t>(150));
    CHECK(result <= Speed::MAX);
}

TEST_CASE("Bug 11b – Speed::scale(maxSpeed, 200) must not exceed maxSpeed",
          "[speed][bug11]") {
    speed_t result = Speed::scale(Speed::MAX, static_cast<proximity_t>(200));
    CHECK(result <= Speed::MAX);
}

TEST_CASE("Bug 11c – Speed::scale: any proximity > 100 must clamp to maxSpeed",
          "[speed][bug11]") {
    for (proximity_t p : {static_cast<proximity_t>(101),
                          static_cast<proximity_t>(150),
                          static_cast<proximity_t>(255),
                          static_cast<proximity_t>(1000)}) {
        speed_t result = Speed::scale(Speed::MAX, p);
        INFO("proximity = " << p);
        CHECK(result <= Speed::MAX);
    }
}
