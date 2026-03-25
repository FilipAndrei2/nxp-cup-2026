/* test/test_vectors.cpp
 * Tests for ls::Vectors – static utility functions for vector collections.
 */
#include "catch2/catch_amalgamated.hpp"
#include "math/Vectors.hpp"

#include <memory>
#include <vector>

using namespace ls;
using Catch::Approx;

// ── Vectors::NORTH and ZERO constants ────────────────────────────────────────

TEST_CASE("Vectors::ZERO is (0, 0)", "[vectors]") {
    CHECK(Vectors::ZERO.getX() == Approx(0.0f));
    CHECK(Vectors::ZERO.getY() == Approx(0.0f));
}

TEST_CASE("Vectors::NORTH is (0, 1)", "[vectors]") {
    // Defined in src/Vectors.cpp as FVector2(0.0f, 1.0f)
    CHECK(Vectors::NORTH.getX() == Approx(0.0f));
    CHECK(Vectors::NORTH.getY() == Approx(1.0f));
}

// ── Vectors::isFinishLine ────────────────────────────────────────────────────

TEST_CASE("Vectors::isFinishLine – exactly horizontal (y == 0)", "[vectors]") {
    CHECK(Vectors::isFinishLine(FVector2(1.0f, 0.0f)));
    CHECK(Vectors::isFinishLine(FVector2(-5.0f, 0.0f)));
}

TEST_CASE("Vectors::isFinishLine – y within threshold (|y| < 0.1)", "[vectors]") {
    CHECK(Vectors::isFinishLine(FVector2(1.0f,  0.05f)));
    CHECK(Vectors::isFinishLine(FVector2(1.0f, -0.05f)));
}

TEST_CASE("Vectors::isFinishLine – y exactly at threshold (|y| == 0.1) is NOT a finish line", "[vectors]") {
    // The implementation uses abs(y) <= threshold, so 0.1 should qualify
    CHECK(Vectors::isFinishLine(FVector2(1.0f, 0.1f)));
    CHECK(Vectors::isFinishLine(FVector2(1.0f, -0.1f)));
}

TEST_CASE("Vectors::isFinishLine – y beyond threshold", "[vectors]") {
    CHECK_FALSE(Vectors::isFinishLine(FVector2(1.0f,  0.11f)));
    CHECK_FALSE(Vectors::isFinishLine(FVector2(1.0f, -0.11f)));
    CHECK_FALSE(Vectors::isFinishLine(FVector2(0.0f,  1.0f)));
}

// ── Vectors::numberOfFinishLineVectors ───────────────────────────────────────

TEST_CASE("Vectors::numberOfFinishLineVectors – empty list returns 0", "[vectors]") {
    std::vector<FVector2> empty;
    CHECK(Vectors::numberOfFinishLineVectors(empty) == 0);
}

TEST_CASE("Vectors::numberOfFinishLineVectors – all non-finish vectors", "[vectors]") {
    std::vector<FVector2> vecs = {
        FVector2(1.0f, 0.5f),
        FVector2(0.0f, 1.0f),
        FVector2(-1.0f, 0.5f),
    };
    CHECK(Vectors::numberOfFinishLineVectors(vecs) == 0);
}

TEST_CASE("Vectors::numberOfFinishLineVectors – all finish-line vectors", "[vectors]") {
    std::vector<FVector2> vecs = {
        FVector2(1.0f,  0.0f),
        FVector2(2.0f, -0.05f),
        FVector2(3.0f,  0.09f),
    };
    CHECK(Vectors::numberOfFinishLineVectors(vecs) == 3);
}

TEST_CASE("Vectors::numberOfFinishLineVectors – mixed", "[vectors]") {
    std::vector<FVector2> vecs = {
        FVector2(1.0f,  0.0f),   // finish
        FVector2(0.0f,  0.5f),   // not finish
        FVector2(2.0f, -0.05f),  // finish
        FVector2(1.0f,  1.0f),   // not finish
    };
    CHECK(Vectors::numberOfFinishLineVectors(vecs) == 2);
}

// ── Vectors::seeingFinishLine ────────────────────────────────────────────────
// FINISH_LINE_NUMBER_OF_VECTORS_THSH = 1

TEST_CASE("Vectors::seeingFinishLine – no finish vectors → false", "[vectors]") {
    std::vector<FVector2> vecs = { FVector2(0.0f, 1.0f) };
    CHECK_FALSE(Vectors::seeingFinishLine(vecs));
}

TEST_CASE("Vectors::seeingFinishLine – one finish vector meets threshold → true", "[vectors]") {
    std::vector<FVector2> vecs = { FVector2(1.0f, 0.0f) };
    CHECK(Vectors::seeingFinishLine(vecs));
}

TEST_CASE("Vectors::seeingFinishLine – multiple finish vectors → true", "[vectors]") {
    std::vector<FVector2> vecs = {
        FVector2(1.0f, 0.0f),
        FVector2(2.0f, 0.0f),
    };
    CHECK(Vectors::seeingFinishLine(vecs));
}

// ── Vectors::notSeeingFinishLine ─────────────────────────────────────────────

TEST_CASE("Vectors::notSeeingFinishLine – no finish vectors → true", "[vectors]") {
    std::vector<FVector2> vecs = { FVector2(0.0f, 1.0f) };
    CHECK(Vectors::notSeeingFinishLine(vecs));
}

TEST_CASE("Vectors::notSeeingFinishLine – has finish vector → false", "[vectors]") {
    std::vector<FVector2> vecs = { FVector2(1.0f, 0.0f) };
    CHECK_FALSE(Vectors::notSeeingFinishLine(vecs));
}

TEST_CASE("Vectors::notSeeingFinishLine – empty list → true", "[vectors]") {
    std::vector<FVector2> vecs;
    CHECK(Vectors::notSeeingFinishLine(vecs));
}
