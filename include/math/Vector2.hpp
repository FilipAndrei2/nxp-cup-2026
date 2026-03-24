/**
 * @file Vector2.hpp
 * @brief Definește clasa template pentru un vector bidimensional.
 */
#pragma once
#include "math/Floats.hpp"
#include "utils/lifesource.hpp"
#include <cmath>
#include <optional>
#include <type_traits>

namespace ls {

/**
 * @brief Clasă template ce reprezintă un vector bidimensional.
 *
 * Suportă operații matematice standard (adunare, scădere, înmulțire,
 * produs scalar/vectorial, normalizare, calcul de unghi etc.).
 * Parametrul @c T trebuie să fie un tip aritmetic (float, double, int etc.).
 * Deși poate fi folosit cu tipuri fără semn, clasa nu a fost proiectată
 * cu acest scop în minte.
 *
 * @tparam T Tipul elementelor vectorului; trebuie să fie aritmetic.
 * @author Filip Andrei-Robert
 */
template <typename T> class Vector2 {
  static_assert(std::is_arithmetic_v<T>, "T trebuie sa fie aritmetic");

private:
  T x; ///< Componenta pe axa X.
  T y; ///< Componenta pe axa Y.

public:
  /**
   * @brief Construiește un vector cu componentele specificate.
   * @param x Componenta pe axa X (implicit 0).
   * @param y Componenta pe axa Y (implicit 0).
   */
  Vector2(T x = 0, T y = 0) : x(x), y(y) {}

  /**
   * @brief Construiește un vector prin translatarea segmentului cu extremitățile
   *        date la origine.
   * @param x1 Coordonata X a primului punct.
   * @param y1 Coordonata Y a primului punct.
   * @param x2 Coordonata X a celui de-al doilea punct.
   * @param y2 Coordonata Y a celui de-al doilea punct.
   */
  Vector2(T x1, T y1, T x2, T y2) {}

  /**
   * @brief Constructor de copiere.
   * @param other Vectorul sursă.
   */
  Vector2(const Vector2 &other) : x(other.x), y(other.y) {}

  /**
   * @brief Constructor de mutare.
   * @param other Vectorul sursă (componentele sale sunt resetate la 0 după mutare).
   */
  Vector2(Vector2 &&other) : x(other.x), y(other.y) {
    other.x = (T)0;
    other.y = (T)0;
  }

public:
  /// @brief Destructor implicit.
  ~Vector2() = default;

public:
  /**
   * @brief Returnează componenta X.
   * @return Valoarea componentei X.
   */
  T getX() const { return this->x; }

  /**
   * @brief Returnează componenta Y.
   * @return Valoarea componentei Y.
   */
  T getY() const { return this->y; }

  /**
   * @brief Verifică dacă vectorul este normalizat (lungime ~= 1).
   * @return @c true dacă lungimea vectorului este aproximativ egală cu 1.
   */
  bool isNormalized() const { return Floats::eq(this->len(), 1.0f); }

  /**
   * @brief Setează componenta X.
   * @param x Noua valoare a componentei X.
   */
  void setX(T x) { this->x = x; }

  /**
   * @brief Setează componenta Y.
   * @param y Noua valoare a componentei Y.
   */
  void setY(T y) { this->y = y; }

public:
  /**
   * @brief Adună doi vectori.
   * @param lhs Vectorul stâng.
   * @param rhs Vectorul drept.
   * @return Suma celor doi vectori.
   */
  static Vector2 Add(const Vector2 &lhs, const Vector2 &rhs) {
    return Vector2(lhs.x + rhs.x, lhs.y + rhs.y);
  }

  /**
   * @brief Scade doi vectori.
   * @param lhs Vectorul stâng (descăzut).
   * @param rhs Vectorul drept (scăzător).
   * @return Diferența celor doi vectori.
   */
  static Vector2 Sub(const Vector2 &lhs, const Vector2 &rhs) {
    return Vector2(lhs.x - rhs.x, lhs.y - rhs.y);
  }

  /**
   * @brief Înmulțește un vector cu un scalar.
   * @param v Vectorul de înmulțit.
   * @param scalar Scalarul cu care se înmulțește.
   * @return Vectorul scalat.
   */
  static Vector2 Mul(const Vector2 &v, const float scalar) {
    return Vector2(v.x * scalar, v.y * scalar);
  }

  /**
   * @brief Calculează produsul scalar (dot product) a doi vectori.
   * @param lhs Vectorul stâng.
   * @param rhs Vectorul drept.
   * @return Produsul scalar al celor doi vectori.
   */
  static float Dot(const Vector2 &lhs, const Vector2 &rhs) {
    return std::sqrt(lhs.x * rhs.x + lhs.y * rhs.y);
  }

  /**
   * @brief Calculează produsul vectorial (cross product) a doi vectori 2D.
   * @param lhs Vectorul stâng.
   * @param rhs Vectorul drept.
   * @return Componenta Z a produsului vectorial.
   */
  static float Cross(const Vector2 &lhs, const Vector2 &rhs) {
    return lhs.x * rhs.y - lhs.y * rhs.x;
  }

  /**
   * @brief Calculează media aritmetică a doi vectori.
   * @param lhs Vectorul stâng.
   * @param rhs Vectorul drept.
   * @return Vectorul mediu dintre cei doi.
   */
  static Vector2 Avg(const Vector2 &lhs, const Vector2 &rhs) {
    return Vector2((lhs.x + rhs.x) / 2, (lhs.y + rhs.y) / 2);
  }

  /**
   * @brief Calculează unghiul dintre doi vectori.
   *
   * Dacă ambii vectori sunt normalizați, calculul este optimizat prin
   * eliminarea împărțirii la norme.
   *
   * @param lhs Vectorul stâng.
   * @param rhs Vectorul drept.
   * @return Unghiul în radiani dintre cei doi vectori.
   */
  static float AngleBetween(const Vector2 &lhs, const Vector2 &rhs) {
    if (lhs.isNormalized() && rhs.isNormalized()) {
      return std::acos(Vector2::Dot(lhs, rhs));
    } else {
      return std::acos(Vector2::Dot(lhs, rhs) / (lhs.len() * rhs.len()));
    }
  }

public:
  inline static const Vector2 NORTH{1, 0};   ///< Vectorul unitar spre nord.
  inline static const Vector2 EAST{0, 1};    ///< Vectorul unitar spre est.
  inline static const Vector2 SOUTH{-1, 0};  ///< Vectorul unitar spre sud.
  inline static const Vector2 WEST{0, -1};   ///< Vectorul unitar spre vest.
  inline static const Vector2 NULLVECT{0, 0};///< Vectorul nul.

public:
  /**
   * @brief Calculează lungimea (norma euclidiană) a vectorului.
   * @return Lungimea vectorului.
   */
  float len() const { return std::sqrt(x * x + y * y); }

  /**
   * @brief Returnează o copie normalizată a vectorului (lungime = 1).
   * @return Vectorul normalizat.
   */
  Vector2 normalized() const { return Vector2(x / len(), y / len()); }

  /**
   * @brief Normalizează vectorul în loc (modifică componentele direct).
   */
  void normalize() {
    auto l = len();
    x /= l;
    y /= l;
  }

  /**
   * @brief Adună un alt vector la acesta (modificare în loc).
   * @param other Vectorul de adăugat.
   */
  void add(const Vector2 &other) {
    this->x += other.x;
    this->y += other.y;
  }

  /**
   * @brief Scade un alt vector din acesta (modificare în loc).
   * @param other Vectorul de scăzut.
   */
  void sub(const Vector2 &other) {
    this->x -= other.x;
    this->y -= other.y;
  }

  /**
   * @brief Înmulțește vectorul cu un scalar (modificare în loc).
   * @param scalar Scalarul cu care se înmulțesc componentele.
   */
  void mul(const float scalar) {
    this->x *= scalar;
    this->y *= scalar;
  }

  /**
   * @brief Calculează media aritmetică cu un alt vector (modificare în loc).
   * @param other Vectorul față de care se calculează media.
   */
  void avg(const Vector2 &other) {
    this->x = (this->x + other.x) / 2;
    this->y = (this->y + other.y) / 2;
  }

public:
  /**
   * @brief Operator de adunare a doi vectori.
   * @param other Vectorul de adăugat.
   * @return Suma celor doi vectori.
   */
  Vector2 operator+(const Vector2 &other) const {
    return Vector2(this->x + other.x, this->y + other.y);
  }

  /**
   * @brief Operator de scădere a doi vectori.
   * @param other Vectorul de scăzut.
   * @return Diferența celor doi vectori.
   */
  Vector2 operator-(const Vector2 &other) const {
    return Vector2(this->x - other.x, this->y - other.y);
  }

  /**
   * @brief Operator de înmulțire cu un scalar.
   * @param scalar Valoarea scalară.
   * @return Vectorul scalat.
   */
  Vector2 operator*(const float scalar) const {
    return Vector2(x * scalar, y * scalar);
  }

  /**
   * @brief Operator de atribuire prin copiere.
   * @param other Vectorul sursă.
   * @return Referință la vectorul curent.
   */
  Vector2 &operator=(const Vector2 &other) {
    this->x = other.x;
    this->y = other.y;
    return *this;
  }

  /**
   * @brief Operator de atribuire prin mutare.
   * @param other Vectorul sursă.
   * @return Referință la vectorul curent.
   */
  Vector2 &operator=(Vector2 &&other) {
    this->x = other.x;
    this->y = other.y;
    return *this;
  }

  /**
   * @brief Operator de egalitate (cu toleranță epsilon).
   * @param other Vectorul cu care se compară.
   * @return @c true dacă ambele componente sunt aproximativ egale.
   */
  bool operator==(const Vector2 &other) const {
    return Floats::eq(this->x, other.x) && Floats::eq(this->y, other.y);
  }

  /**
   * @brief Operator de inegalitate (cu toleranță epsilon).
   * @param other Vectorul cu care se compară.
   * @return @c true dacă cel puțin o componentă diferă.
   */
  bool operator!=(const Vector2 &other) const {
    return !Floats::eq(this->x, other.x) || !Floats::eq(this->y, other.y);
  }
};

/// @brief Alias pentru vectorul 2D cu tip @c float.
using FVector2 = Vector2<float>;

/// @brief Alias pentru vectorul 2D cu tip @c double.
using DVector2 = Vector2<double>;

} // namespace ls
