/* test/test_angles.cpp
 * Tests for ls::Angles – angle-to-servo-direction conversion.
 *
 * Formula:  dir = (200 / PI) * angle,  clamped to [-PI/2, PI/2]
 * So the range of dir is approximately [-100, 100].
 */
#include "catch2/catch_amalgamated.hpp"
#include "math/Angles.hpp"

using namespace ls;
using Catch::Approx;

static constexpr float PI = Angles::FLOAT_PI;

// ── Basic conversion ─────────────────────────────────────────────────────────

TEST_CASE("Angles::angleToDir – zero angle maps to 0", "[angles]") {
    CHECK(Angles::angleToDir(0.0f) == 0);
}

TEST_CASE("Angles::angleToDir – PI/2 maps to 100", "[angles]") {
    // (200/PI) * (PI/2) = 100
    CHECK(Angles::angleToDir(PI / 2.0f) == 100);
}

TEST_CASE("Angles::angleToDir – -PI/2 maps to -100", "[angles]") {
    // (200/PI) * (-PI/2) = -100
    CHECK(Angles::angleToDir(-PI / 2.0f) == -100);
}

TEST_CASE("Angles::angleToDir – PI/4 maps to 50", "[angles]") {
    // (200/PI) * (PI/4) = 50
    CHECK(Angles::angleToDir(PI / 4.0f) == 50);
}

// ── Clamping ──────────────────────────────────────────────────────────────────

TEST_CASE("Angles::angleToDir – angle beyond PI/2 is clamped to PI/2 → 100", "[angles]") {
    CHECK(Angles::angleToDir(PI) == 100);
    CHECK(Angles::angleToDir(2.0f * PI) == 100);
    CHECK(Angles::angleToDir(PI / 2.0f + 0.01f) == 100);
}

TEST_CASE("Angles::angleToDir – angle below -PI/2 is clamped to -PI/2 → -100", "[angles]") {
    CHECK(Angles::angleToDir(-PI) == -100);
    CHECK(Angles::angleToDir(-2.0f * PI) == -100);
    CHECK(Angles::angleToDir(-PI / 2.0f - 0.01f) == -100);
}

// ── Intermediate values ───────────────────────────────────────────────────────

TEST_CASE("Angles::angleToDir – small positive angle", "[angles]") {
    float angle = PI / 6.0f; // 30 degrees
    int expected = static_cast<int>((200.0f / PI) * angle); // ~33
    CHECK(Angles::angleToDir(angle) == expected);
}

TEST_CASE("Angles::angleToDir – small negative angle", "[angles]") {
    float angle = -PI / 6.0f; // -30 degrees
    int expected = static_cast<int>((200.0f / PI) * angle); // ~-33
    CHECK(Angles::angleToDir(angle) == expected);
}
