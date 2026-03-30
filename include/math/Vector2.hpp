#pragma once
#include "math/Floats.hpp"
#include "utils/lifesource.hpp"
#include <cmath>
#include <optional>
#include <type_traits>

namespace ls {
/**
 * @brief A class representing a 2-dimensional vector
 * The parameter T must be an arithmetic type (float, double, int, long, etc).
 * Although it could be used with unsigned types, the class was not designed
 * with that intend in mind.
 * @author Filip Andrei-Robert
 */
template <typename T> class Vector2 {
  static_assert(std::is_arithmetic_v<T>, "T trebuie sa fie aritmetic");

private: // Membri
  T x, y;

public: // Constructori
  Vector2(T x = 0, T y = 0) : x(x), y(y) {}

  /**
   * Creates the object by translating the segment with said endpoints to
   * origin.
   */
  Vector2(T x1, T y1, T x2, T y2) : x(x2 - x1), y(y2 - y1) {}

  Vector2(const Vector2 &other) : x(other.x), y(other.y) {}

  Vector2(Vector2 &&other) : x(other.x), y(other.y) {
    other.x = (T)0;
    other.y = (T)0;
  }

public: // Destructor
  ~Vector2() = default;

public: // Getteri setteri
  T getX() const { return this->x; }
  T getY() const { return this->y; }

  bool isNormalized() const { return Floats::eq(this->len(), 1.0f); }

  void setX(T x) { this->x = x; }

  void setY(T y) { this->y = y; }

public: // Metode statice
  static Vector2 Add(const Vector2 &lhs, const Vector2 &rhs) {
    return Vector2(lhs.x + rhs.x, lhs.y + rhs.y);
  }

  static Vector2 Sub(const Vector2 &lhs, const Vector2 &rhs) {
    return Vector2(lhs.x - rhs.x, lhs.y - rhs.y);
  }

  static Vector2 Mul(const Vector2 &v, const float scalar) {
    return Vector2(v.x * scalar, v.y * scalar);
  }

  static float Dot(const Vector2 &lhs, const Vector2 &rhs) {
    return (float)(lhs.x * rhs.x + lhs.y * rhs.y);
  }

  static float Cross(const Vector2 &lhs, const Vector2 &rhs) {
    return lhs.x * rhs.y - lhs.y * rhs.x;
  }

  static Vector2 Avg(const Vector2 &lhs, const Vector2 &rhs) {
    return Vector2((lhs.x + rhs.x) / 2, (lhs.y + rhs.y) / 2);
  }

  static float AngleBetween(const Vector2 &lhs, const Vector2 &rhs) {
    // NOTE: Optimizare -> pentru vectori normalizati se simplifica niste norme
    // NOTE: A dot B = A.len * B.len * cos(alph)
    // alph = acos(A dot B/ (A.len* B.len))
    float dot;
    if (lhs.isNormalized() && rhs.isNormalized()) {
      dot = Vector2::Dot(lhs, rhs);
    } else {
      float lenProduct = lhs.len() * rhs.len();
      if (Floats::eq(lenProduct, 0.0f)) {
        return 0.0f; // degenerate: zero-length vector -> no rotation
      }
      dot = Vector2::Dot(lhs, rhs) / lenProduct;
    }
    // Clamp to [-1, 1] to guard against floating-point rounding errors in acos
    if (dot > 1.0f) dot = 1.0f;
    if (dot < -1.0f) dot = -1.0f;

    float angle = std::acos(dot);
    // Use the cross product sign to determine turn direction:
    // Cross(lhs, rhs) > 0 means lhs is to the right of rhs → positive angle (right turn)
    // Cross(lhs, rhs) < 0 means lhs is to the left of rhs → negative angle (left turn)
    if (Vector2::Cross(lhs, rhs) < 0.0f) {
      angle = -angle;
    }
    return angle;
  }

public: // Membri statici
  inline static const Vector2 NORTH{0, 1};

  inline static const Vector2 EAST{1, 0};

  inline static const Vector2 SOUTH{0, -1};

  inline static const Vector2 WEST{-1, 0};

  inline static const Vector2 NULLVECT{0, 0};

public: // Metode instanta
  float len() const { return std::sqrt(x * x + y * y); }

  Vector2 normalized() const { return Vector2(x / len(), y / len()); }

  void normalize() {
    auto l = len();
    x /= l;
    y /= l;
  }

  void add(const Vector2 &other) {
    this->x += other.x;
    this->y += other.y;
  }

  void sub(const Vector2 &other) {
    this->x -= other.x;
    this->y -= other.y;
  }

  void mul(const float scalar) {
    this->x *= scalar;
    this->y *= scalar;
  }

  void avg(const Vector2 &other) {
    this->x = (this->x + other.x) / 2;
    this->y = (this->y + other.y) / 2;
  }

public: // Operatori
  Vector2 operator+(const Vector2 &other) const {
    return Vector2(this->x + other.x, this->y + other.y);
  }

  Vector2 operator-(const Vector2 &other) const {
    return Vector2(this->x - other.x, this->y - other.y);
  }

  Vector2 operator*(const float scalar) const {
    return Vector2(x * scalar, y * scalar);
  }

  Vector2 &operator=(const Vector2 &other) {
    this->x = other.x;
    this->y = other.y;
    return *this;
  }

  Vector2 &operator=(Vector2 &&other) {
    this->x = other.x;
    this->y = other.y;
    return *this;
  }

  bool operator==(const Vector2 &other) const {

    return Floats::eq(this->x, other.x) && Floats::eq(this->y, other.y);
  }

  bool operator!=(const Vector2 &other) const {
    return !Floats::eq(this->x, other.x) || !Floats::eq(this->y, other.y);
  }
};

using FVector2 = Vector2<float>;
using DVector2 = Vector2<double>;
} // namespace ls
