#include "catch2/catch_amalgamated.hpp"
#include "math/Vector2.hpp"

#include <cmath>
#include <numbers>

using namespace ls;
using Catch::Approx;

static constexpr float PI = 3.14159265358979323846f;

// ── Construction ─────────────────────────────────────────────────────────────

TEST_CASE("Vector2 – default constructor produces zero vector", "[vector2]") {
  FVector2 v;
  CHECK(v.getX() == Approx(0.0f));
  CHECK(v.getY() == Approx(0.0f));
}

TEST_CASE("Vector2 – parameterised constructor", "[vector2]") {
  FVector2 v(3.0f, 4.0f);
  CHECK(v.getX() == Approx(3.0f));
  CHECK(v.getY() == Approx(4.0f));
}

TEST_CASE("Vector2 – copy constructor", "[vector2]") {
  FVector2 a(1.0f, 2.0f);
  FVector2 b(a);
  CHECK(b.getX() == Approx(1.0f));
  CHECK(b.getY() == Approx(2.0f));
}

TEST_CASE("Vector2 – move constructor zeroes the source", "[vector2]") {
  FVector2 a(5.0f, 6.0f);
  FVector2 b(std::move(a));
  CHECK(b.getX() == Approx(5.0f));
  CHECK(b.getY() == Approx(6.0f));
  CHECK(a.getX() == Approx(0.0f));
  CHECK(a.getY() == Approx(0.0f));
}

// ── Static cardinal directions
// ────────────────────────────────────────────────

TEST_CASE("Vector2 – static cardinal constants", "[vector2]") {
  CHECK(FVector2::NORTH.getX() == Approx(0.0f));
  CHECK(FVector2::NORTH.getY() == Approx(1.0f));

  CHECK(FVector2::EAST.getX() == Approx(1.0f));
  CHECK(FVector2::EAST.getY() == Approx(0.0f));

  CHECK(FVector2::SOUTH.getX() == Approx(0.0f));
  CHECK(FVector2::SOUTH.getY() == Approx(-1.0f));

  CHECK(FVector2::WEST.getX() == Approx(-1.0f));
  CHECK(FVector2::WEST.getY() == Approx(0.0f));

  CHECK(FVector2::NULLVECT.getX() == Approx(0.0f));
  CHECK(FVector2::NULLVECT.getY() == Approx(0.0f));
}

// ── Magnitude
// ─────────────────────────────────────────────────────────────────

TEST_CASE("Vector2::len – 3-4-5 Pythagorean triple", "[vector2]") {
  FVector2 v(3.0f, 4.0f);
  CHECK(v.len() == Approx(5.0f));
}

TEST_CASE("Vector2::len – unit vectors", "[vector2]") {
  CHECK(FVector2(1.0f, 0.0f).len() == Approx(1.0f));
  CHECK(FVector2(0.0f, 1.0f).len() == Approx(1.0f));
}

TEST_CASE("Vector2::len – zero vector", "[vector2]") {
  CHECK(FVector2(0.0f, 0.0f).len() == Approx(0.0f));
}

TEST_CASE("Vector2::len – negative components", "[vector2]") {
  CHECK(FVector2(-3.0f, -4.0f).len() == Approx(5.0f));
}

// ── Normalisation
// ─────────────────────────────────────────────────────────────

TEST_CASE("Vector2::normalized – returns unit vector", "[vector2]") {
  FVector2 v(3.0f, 4.0f);
  FVector2 n = v.normalized();
  CHECK(n.len() == Approx(1.0f).epsilon(1e-5f));
  CHECK(n.getX() == Approx(0.6f).epsilon(1e-5f));
  CHECK(n.getY() == Approx(0.8f).epsilon(1e-5f));
}

TEST_CASE("Vector2::isNormalized – unit vectors", "[vector2]") {
  CHECK(FVector2(1.0f, 0.0f).isNormalized());
  CHECK(FVector2(0.0f, 1.0f).isNormalized());
  FVector2 d(1.0f / std::sqrt(2.0f), 1.0f / std::sqrt(2.0f));
  CHECK(d.isNormalized());
}

TEST_CASE("Vector2::isNormalized – non-unit vector", "[vector2]") {
  CHECK_FALSE(FVector2(3.0f, 4.0f).isNormalized());
  CHECK_FALSE(FVector2(2.0f, 0.0f).isNormalized());
}

TEST_CASE("Vector2::normalize – mutates to unit vector", "[vector2]") {
  FVector2 v(0.0f, 5.0f);
  v.normalize();
  CHECK(v.len() == Approx(1.0f).epsilon(1e-5f));
  CHECK(v.getY() == Approx(1.0f).epsilon(1e-5f));
}

// ── Arithmetic static methods
// ─────────────────────────────────────────────────

TEST_CASE("Vector2::Add", "[vector2]") {
  FVector2 a(1.0f, 2.0f), b(3.0f, 4.0f);
  FVector2 r = FVector2::Add(a, b);
  CHECK(r.getX() == Approx(4.0f));
  CHECK(r.getY() == Approx(6.0f));
}

TEST_CASE("Vector2::Sub", "[vector2]") {
  FVector2 a(5.0f, 3.0f), b(1.0f, 2.0f);
  FVector2 r = FVector2::Sub(a, b);
  CHECK(r.getX() == Approx(4.0f));
  CHECK(r.getY() == Approx(1.0f));
}

TEST_CASE("Vector2::Mul – scalar multiplication", "[vector2]") {
  FVector2 v(2.0f, 3.0f);
  FVector2 r = FVector2::Mul(v, 2.0f);
  CHECK(r.getX() == Approx(4.0f));
  CHECK(r.getY() == Approx(6.0f));
}

TEST_CASE("Vector2::Mul – by zero", "[vector2]") {
  FVector2 v(2.0f, 3.0f);
  FVector2 r = FVector2::Mul(v, 0.0f);
  CHECK(r.getX() == Approx(0.0f));
  CHECK(r.getY() == Approx(0.0f));
}

TEST_CASE("Vector2::Avg – midpoint", "[vector2]") {
  FVector2 a(0.0f, 0.0f), b(4.0f, 6.0f);
  FVector2 m = FVector2::Avg(a, b);
  CHECK(m.getX() == Approx(2.0f));
  CHECK(m.getY() == Approx(3.0f));
}

// ── Cross product
// ─────────────────────────────────────────────────────────────

TEST_CASE("Vector2::Cross – orthogonal unit vectors", "[vector2]") {
  // (1,0) × (0,1) = 1*1 - 0*0 = 1
  CHECK(FVector2::Cross(FVector2(1.0f, 0.0f), FVector2(0.0f, 1.0f)) ==
        Approx(1.0f));
  // (0,1) × (1,0) = 0*0 - 1*1 = -1
  CHECK(FVector2::Cross(FVector2(0.0f, 1.0f), FVector2(1.0f, 0.0f)) ==
        Approx(-1.0f));
}

TEST_CASE("Vector2::Cross – parallel vectors give zero", "[vector2]") {
  CHECK(FVector2::Cross(FVector2(1.0f, 0.0f), FVector2(2.0f, 0.0f)) ==
        Approx(0.0f));
  CHECK(FVector2::Cross(FVector2(1.0f, 1.0f), FVector2(2.0f, 2.0f)) ==
        Approx(0.0f));
}

TEST_CASE("Vector2::Dot – parallel unit vectors (implementation behaviour)",
          "[vector2][dot]") {
  CHECK(FVector2::Dot(FVector2(1.0f, 0.0f), FVector2(1.0f, 0.0f)) ==
        Approx(1.0f));
}

TEST_CASE("Vector2::Dot – orthogonal unit vectors (implementation behaviour)",
          "[vector2][dot]") {
  CHECK(FVector2::Dot(FVector2(1.0f, 0.0f), FVector2(0.0f, 1.0f)) ==
        Approx(0.0f));
}

TEST_CASE("Vector2::Dot – anti-parallel vectors expose sqrt bug",
          "[vector2][dot]") {
  // True dot product of (1,0) · (-1,0) = -1
  float result = FVector2::Dot(FVector2(1.0f, 0.0f), FVector2(-1.0f, 0.0f));
  CHECK(result == Approx(-1.0f));
}

TEST_CASE("Vector2::Dot - implementation behaviour", "[vector2][dot]") {
  CHECK(FVector2::Dot(FVector2(6.7f, 200.0f), FVector2(6.0f, 9.0f)) ==
        Approx(6.7f * 6.0f + 200.0f * 9.0f));
}

// ── Angle between
// ─────────────────────────────────────────────────────────────

TEST_CASE("Vector2::AngleBetween – same direction (angle = 0)",
          "[vector2][angle]") {
  // For normalised parallel vectors: Dot = 1, acos(1) = 0
  FVector2 a(1.0f, 0.0f), b(1.0f, 0.0f);
  CHECK(FVector2::AngleBetween(a, b) == Approx(0.0f).margin(1e-5f));
}

TEST_CASE("Vector2::AngleBetween – perpendicular unit vectors (angle = PI/2)",
          "[vector2][angle]") {
  // Dot({1,0},{0,1}) = 0, acos(0) = PI/2
  FVector2 a(1.0f, 0.0f), b(0.0f, 1.0f);
  CHECK(FVector2::AngleBetween(a, b) == Approx(PI / 2.0f).epsilon(1e-5f));
}

// ── Signed angle (BUG-4 regression: was always non-negative) ─────────────────

TEST_CASE("Vector2::AngleBetween – right-of-reference gives positive angle",
          "[vector2][angle]") {
  // Track vector pointing right-forward; NORTH is reference.
  // Cross((1,0), (0,1)) = 1 > 0 → positive angle (right turn needed).
  FVector2 east(1.0f, 0.0f), north(0.0f, 1.0f);
  CHECK(FVector2::AngleBetween(east, north) == Approx(PI / 2.0f).epsilon(1e-5f));
}

TEST_CASE("Vector2::AngleBetween – left-of-reference gives negative angle",
          "[vector2][angle]") {
  // Track vector pointing left-forward; NORTH is reference.
  // Cross((-1,0), (0,1)) = -1 < 0 → negative angle (left turn needed).
  FVector2 west(-1.0f, 0.0f), north(0.0f, 1.0f);
  CHECK(FVector2::AngleBetween(west, north) == Approx(-PI / 2.0f).epsilon(1e-5f));
}

TEST_CASE("Vector2::AngleBetween – diagonal right gives positive angle",
          "[vector2][angle]") {
  // Vector at 45° right of north should give ~PI/4 (positive).
  float s = 1.0f / std::sqrt(2.0f);
  FVector2 diag(s, s), north(0.0f, 1.0f);
  CHECK(FVector2::AngleBetween(diag, north) == Approx(PI / 4.0f).epsilon(1e-4f));
}

TEST_CASE("Vector2::AngleBetween – diagonal left gives negative angle",
          "[vector2][angle]") {
  // Vector at 45° left of north should give ~-PI/4 (negative).
  float s = 1.0f / std::sqrt(2.0f);
  FVector2 diag(-s, s), north(0.0f, 1.0f);
  CHECK(FVector2::AngleBetween(diag, north) == Approx(-PI / 4.0f).epsilon(1e-4f));
}

// ── NaN guard – zero-length vector (BUG-7 regression) ────────────────────────

TEST_CASE("Vector2::AngleBetween – zero-length lhs returns 0 (no NaN)",
          "[vector2][angle]") {
  FVector2 zero(0.0f, 0.0f), north(0.0f, 1.0f);
  float result = FVector2::AngleBetween(zero, north);
  CHECK(result == Approx(0.0f).margin(1e-5f));
}

TEST_CASE("Vector2::AngleBetween – anti-parallel unit vectors return PI (no NaN)",
          "[vector2][angle]") {
  // Dot product = -1; acos(-1) = PI. Cross = 0 → sign stays positive.
  FVector2 a(0.0f, 1.0f), b(0.0f, -1.0f);
  CHECK(FVector2::AngleBetween(a, b) == Approx(PI).epsilon(1e-4f));
}

// ── 4-argument constructor (BUG-6 regression: body was empty) ────────────────

TEST_CASE("Vector2 – 4-argument constructor creates direction vector from two endpoints",
          "[vector2]") {
  // From (1, 2) to (4, 6): direction vector = (4-1, 6-2) = (3, 4)
  FVector2 v(1.0f, 2.0f, 4.0f, 6.0f);
  CHECK(v.getX() == Approx(3.0f));
  CHECK(v.getY() == Approx(4.0f));
}

TEST_CASE("Vector2 – 4-argument constructor with coincident points gives zero vector",
          "[vector2]") {
  FVector2 v(3.0f, 3.0f, 3.0f, 3.0f);
  CHECK(v.getX() == Approx(0.0f));
  CHECK(v.getY() == Approx(0.0f));
}

// ── Operators
// ─────────────────────────────────────────────────────────────────

TEST_CASE("Vector2::operator+ – component-wise addition", "[vector2]") {
  FVector2 a(1.0f, 2.0f), b(3.0f, 4.0f);
  FVector2 r = a + b;
  CHECK(r.getX() == Approx(4.0f));
  CHECK(r.getY() == Approx(6.0f));
}

TEST_CASE("Vector2::operator- – component-wise subtraction", "[vector2]") {
  FVector2 a(5.0f, 3.0f), b(2.0f, 1.0f);
  FVector2 r = a - b;
  CHECK(r.getX() == Approx(3.0f));
  CHECK(r.getY() == Approx(2.0f));
}

TEST_CASE("Vector2::operator* – scalar multiplication", "[vector2]") {
  FVector2 v(2.0f, 3.0f);
  FVector2 r = v * 3.0f;
  CHECK(r.getX() == Approx(6.0f));
  CHECK(r.getY() == Approx(9.0f));
}

TEST_CASE("Vector2::operator== – equal vectors", "[vector2]") {
  CHECK(FVector2(1.0f, 2.0f) == FVector2(1.0f, 2.0f));
  CHECK(FVector2(0.0f, 0.0f) == FVector2(0.0f, 0.0f));
}

TEST_CASE("Vector2::operator== – uses Floats::eq epsilon", "[vector2]") {
  // Vectors within floating-point epsilon are considered equal
  CHECK(FVector2(0.0f, 0.0f) == FVector2(5e-7f, 5e-7f));
}

TEST_CASE("Vector2::operator!= – different vectors", "[vector2]") {
  CHECK(FVector2(1.0f, 0.0f) != FVector2(0.0f, 1.0f));
  CHECK(FVector2(1.0f, 2.0f) != FVector2(1.0f, 3.0f));
}

TEST_CASE("Vector2::operator= – copy assignment", "[vector2]") {
  FVector2 a(1.0f, 2.0f);
  FVector2 b(9.0f, 9.0f);
  b = a;
  CHECK(b.getX() == Approx(1.0f));
  CHECK(b.getY() == Approx(2.0f));
}

// ── Mutating instance methods
// ─────────────────────────────────────────────────

TEST_CASE("Vector2::add – in-place addition", "[vector2]") {
  FVector2 a(1.0f, 2.0f);
  a.add(FVector2(3.0f, 4.0f));
  CHECK(a.getX() == Approx(4.0f));
  CHECK(a.getY() == Approx(6.0f));
}

TEST_CASE("Vector2::sub – in-place subtraction", "[vector2]") {
  FVector2 a(5.0f, 5.0f);
  a.sub(FVector2(2.0f, 3.0f));
  CHECK(a.getX() == Approx(3.0f));
  CHECK(a.getY() == Approx(2.0f));
}

TEST_CASE("Vector2::mul – in-place scalar multiplication", "[vector2]") {
  FVector2 v(2.0f, 4.0f);
  v.mul(0.5f);
  CHECK(v.getX() == Approx(1.0f));
  CHECK(v.getY() == Approx(2.0f));
}

TEST_CASE("Vector2::avg – in-place midpoint", "[vector2]") {
  FVector2 a(0.0f, 0.0f);
  a.avg(FVector2(4.0f, 6.0f));
  CHECK(a.getX() == Approx(2.0f));
  CHECK(a.getY() == Approx(3.0f));
}

TEST_CASE("Vector2 – setX / setY", "[vector2]") {
  FVector2 v;
  v.setX(7.0f);
  v.setY(8.0f);
  CHECK(v.getX() == Approx(7.0f));
  CHECK(v.getY() == Approx(8.0f));
}

// ── Move assignment operator ──────────────────────────────────────────────────

TEST_CASE("Vector2::operator= – move assignment does NOT zero the source", "[vector2]") {
    // Unlike the move constructor, the move assignment operator copies but
    // does not clear the moved-from object's components.
    FVector2 a(3.0f, 4.0f);
    FVector2 b;
    b = std::move(a);
    CHECK(b.getX() == Approx(3.0f));
    CHECK(b.getY() == Approx(4.0f));
    // Source is NOT zeroed (move constructor zeros it, but operator= does not)
    CHECK(a.getX() == Approx(3.0f));
    CHECK(a.getY() == Approx(4.0f));
}


