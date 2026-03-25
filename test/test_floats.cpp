/* test/test_floats.cpp
 * Tests for ls::Floats – floating-point equality with epsilon tolerance.
 */
#include "catch2/catch_amalgamated.hpp"
#include "math/Floats.hpp"

using namespace ls;

TEST_CASE("Floats::eq – identical values", "[floats]") {
    CHECK(Floats::eq(0.0f, 0.0f));
    CHECK(Floats::eq(1.0f, 1.0f));
    CHECK(Floats::eq(-5.0f, -5.0f));
    CHECK(Floats::eq(3.14f, 3.14f));
}

TEST_CASE("Floats::eq – values within epsilon are equal", "[floats]") {
    // FLOAT_EPS = 1e-6; values closer than that must be considered equal
    CHECK(Floats::eq(0.0f, 5e-7f));
    CHECK(Floats::eq(1.0f, 1.0f + 5e-7f));
    CHECK(Floats::eq(-1.0f, -1.0f + 5e-7f));
}

TEST_CASE("Floats::eq – values at or beyond epsilon are not equal", "[floats]") {
    // 1e-6 is exactly the boundary – the implementation uses strict less-than
    CHECK_FALSE(Floats::eq(0.0f, 1e-6f));
    CHECK_FALSE(Floats::eq(0.0f, 2e-6f));
    CHECK_FALSE(Floats::eq(1.0f, 2.0f));
    CHECK_FALSE(Floats::eq(-1.0f, 1.0f));
}

TEST_CASE("Floats::eq – symmetry", "[floats]") {
    CHECK(Floats::eq(0.5f, 0.5f + 5e-7f) == Floats::eq(0.5f + 5e-7f, 0.5f));
    CHECK(Floats::eq(0.0f, 1.0f) == Floats::eq(1.0f, 0.0f));
}
