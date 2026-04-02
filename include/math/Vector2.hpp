#pragma once
#include "math/Floats.hpp"
#include "utils/lifesource.hpp"
#include <cmath>
#include <optional>
#include <type_traits>
#include "math/Angles.hpp"

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

  Vector2(const Vector2 &other) : x(other.x), y(other.y) {}

  Vector2(Vector2 &&other) : x(other.x), y(other.y) {
    other.x = (T)0;
    other.y = (T)0;
  }

  // Creaza vectorul translatand segmentul in origine in cadranele 1 si 2
  Vector2(T x0, T y0, T x1, T y1) {
    this->x = x0 > x1 ? x0 - x1 : x1 - x0;
    this->y = y0 > y1 ? y0 - y1 : y1 - y0;
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

  static Vector2 Avg(const Vector2 &lhs, const Vector2 &rhs) {
    return Vector2((lhs.x/((T)2)+ rhs.x/((T)2)), (lhs.y/((T)2) + rhs.y/((T)2)));
  }

  /**
   * @returns Direct input for Servo, [-100, 100]
   */
  static int AngleToSteer(const Vector2 &vect) {
   // A dot B = ||A|| * ||B|| * cos(alph)
   // alph = acos(A dot B / ( ||A|| *||B||))
   // vectori normalizati -> ||A|| == ||B|| == 1
   // A = (0, 1)
   // alph = acos(v.y)

	  auto alpha = std::acos(vect.y); // [0, PI/2]

    int sign = (vect.x >= 0) ? 1 : -1; // Daca vectorul se afla in cadranul 2
    if (sign) {
      alpha = -alpha;
    }
    return Angles::angleToDir(alpha); // output intre [-100. 100]
  }


public: // Membri statici
  inline static const Vector2 NORTH{0, 1};

  inline static const Vector2 NULLVECT{0, 0};

public: // Metode instanta
  float len() const { return std::sqrt(x * x + y * y); }

  Vector2 normalized() const {
    auto l = len();
    if (l == 0.0f) return *this;
    return Vector2(x / l, y / l);
  }

  void normalize() {
    auto length = len();
    if (Floats::eq(length, 0.0f)) return;
    x /= length;
    y /= length;
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
