#pragma once
#include <type_traits>
#include <cmath>
#include <optional>

/**
 * @brief A class representing a 2-dimensional vector
 * @author Filip Andrei-Robert
*/
template <typename T>
class Vector2 {
static_assert(std::is_arithmetic_v<T>, "T trebuie sa fie aritmetic");
private: // Membri
    T x, y;

public: // Constructori
    Vector2(T x = 0, T y = 0)
        : x(x), y(y)
    { }

    Vector2(const Vector2& other)
        : x(other.x), y(other.y)
    { }

    Vector2(Vector2&& other)
        : x(other.x), y(other.y)
    {
        other.x = (T)0;
        other.y = (T)0;
    }

public: // Destructor
    ~Vector2() = default;

public: // Getteri setteri
    T getX() { return this->x; }
    T getY() { return this->y; }

    bool isNormalized() {
        if (this->cache.isNormalized.has_value()) {
            return this->cache.isNormalized.value();
        }
        bool res = (1.0f - this->len()) <= std::numeric_limits<float>::epsilon();
        this->cache.isNormalized = res;
        return res;
    }

    void setX(T x) { this->x = x; }
    void setY(T y) { this->y = y; }

public: // Metode statice
    static Vector2 Add(const Vector2& lhs, const Vector2& rhs) {
        return Vector2(lhs.x + rhs.x, lhs.y + rhs.y);
    }

    static Vector2 Sub(const Vector2& lhs, const Vector2& rhs) {
        return Vector2(lhs.x - rhs.x, lhs.y - rhs.y);
    }

    static Vector2 Mul(const Vector2& v, const float scalar) {
        return Vector2(v.x * scalar, v.y * scalar);    
    }

    static float Dot(const Vector2& lhs, const Vector2& rhs) {
        return std::sqrt(lhs.x * rhs.x + lhs.y * rhs.y);
    }

    static float Cross(const Vector2& lhs, const Vector2& rhs) {
        return lhs.x * rhs.y - lhs.y * rhs.x; 
    }

public: // Membri statici
    static const Vector2 NORTH;
    static const Vector2 EAST;
    static const Vector2 SOUTH;
    static const Vector2 WEST;
    static const Vector2 NULLVECT;

private: // Metode private
    void clearCache(void) {
        this->cache.isNormalized = std::nullopt;
    }
    
    void computeCache(void) {
        if (!cache.len.has_calue()) {
            cache.len = this->len();
        }

        if (!cache.isNormalized.has_value()) {
            cache.isNormalized = this->isNormalized();
        }
    }

public: // Metode instanta 
    float len() const {
        if (cache.len.has_value()) {
            return cache.len.value();
        }

        float res = std::sqrt(x*x + y*y);
        cache.len = res;
        return res;
    }

    Vector2 normalized() const {
        return Vector2(x/len(), y/len());
    }
    
    void normalize() {
        auto l = len();
        x /= l;
        y /= l;
    }

public: // Operatori
    Vector2 operator + (const Vector2& other) const {
        return Vector2(this->x + other.x, this->y + other.y);
    }

    Vector2 operator - (const Vector2& other) const {
        return Vector2(this->x - other.x, this->y - other.y);
    }

    Vector2 operator * (const float scalar) const {
        return Vector2(x*scalar, y*scalar);
    }

private: // Clasa ajutatoare cache
    struct Cache {
        std::optional<bool> isNormalized;
        std::optional<float> len;
    };

    Vector2::Cache cache;
};

template <typename T>
const Vector2<T> Vector2<T>::NORTH(1, 0);

template <typename T>
const Vector2<T> Vector2<T>::SOUTH(-1, 0);

template <typename T>
const Vector2<T> Vector2<T>::EAST(0, 1);

template <typename T>
const Vector2<T> Vector2<T>::WEST(0, -1);

template <typename T>
const Vector2<T> Vector2<T>::NULLVECT(0, 0);

using FVector2 = Vector2<float>;
using DVector2 = Vector2<double>;
