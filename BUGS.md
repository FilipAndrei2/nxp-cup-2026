# Bug Report – NXP Cup 2026

> **Scope:** Only `.cpp` and `.hpp` files are analysed here.  
> Bugs are listed from highest to lowest severity.

---

## BUG-1 · CRITICAL — Wrong angle formula in `OnTrackState` for 3+ vectors

**File:** `src/OnTrackState.cpp`  
**Function:** `computeSpeedAndAngle` – `default` case (3 or more info-vectors)

### Description
When three or more track vectors are detected the algorithm is supposed to:

1. Compute the average of the first two vectors (`medi`).
2. Compute the angle between `medi` and `Vectors::NORTH`.

Step 1 is correct but step 2 is not – the angle is computed between the two
**raw** vectors instead of between `medi` and `NORTH`.  `medi` is computed
and immediately discarded.

### Buggy code
```cpp
// default case in computeSpeedAndAngle (OnTrackState.cpp ~line 73)
FVector2 medi = Vector2<float>::Avg(inInfoVectors[0], inInfoVectors[1]);
outAngle = Vector2<float>::AngleBetween(inInfoVectors[0], inInfoVectors[1]); // ← BUG
outSpeed = Speed::scale(Speed::MAX, outAngle);
```

### Expected code
```cpp
FVector2 medi = Vector2<float>::Avg(inInfoVectors[0], inInfoVectors[1]);
outAngle = Vector2<float>::AngleBetween(medi, Vectors::NORTH); // consistent with case 2
outSpeed = Speed::scale(Speed::MAX, outAngle);
```

### Impact
The steering angle is wrong whenever three or more track vectors are in view.
For two symmetric vectors pointing equally left and right of NORTH the average
correctly points NORTH (angle = 0, full speed), but the bug produces an angle
of ~π/2 (speed ≈ 0) – the car stops or crawls instead of driving straight.
This is the most common multi-vector scenario on a straight track.

---

## BUG-2 · HIGH — Integer division in `Speed::scale(speed_t, proximity_t)` always returns 0 for proximity 1–99

**File:** `include/params/speed.hpp`  
**Function:** `Speed::scale(speed_t maxSpeed, proximity_t cubeProxi)`

### Description
`proximity_t` is `uint32_t`.  The expression `cubeProxi / 100` therefore uses
**integer division** and evaluates to `0` for every input in the range 1–99.
Only the exact value `100` yields `1`, so the function is effectively binary:
it returns `0` or `maxSpeed`, with nothing in between.

### Buggy code
```cpp
static speed_t scale(const speed_t maxSpeed, const proximity_t cubeProxi) {
    return maxSpeed * (cubeProxi / 100); // integer division – result is 0 for cubeProxi < 100
}
```

### Expected code
```cpp
static speed_t scale(const speed_t maxSpeed, const proximity_t cubeProxi) {
    return static_cast<speed_t>(maxSpeed * (cubeProxi / 100.0f)); // floating-point division
}
```

### Impact
Any state that scales speed by cube proximity
(`SeeingFinishLineSecondTimeState`, `WaitingToApproachCubeState`) will set
speed to **zero** as long as proximity is not exactly 100.  The intended
gradual slow-down as the car approaches the cube never happens.

---

## BUG-3 · HIGH — `Vector2` four-argument constructor has an empty body

**File:** `include/math/Vector2.hpp`  
**Constructor:** `Vector2(T x1, T y1, T x2, T y2)`

### Description
The comment above the constructor states that it should create a vector by
translating the segment with endpoints `(x1, y1)` and `(x2, y2)` to the
origin – i.e. it should produce the direction vector `(x2-x1, y2-y1)`.
The body is empty so the member variables `x` and `y` are
**default-initialised to 0**, silently producing a zero vector.

### Buggy code
```cpp
Vector2(T x1, T y1, T x2, T y2) {} // members x, y left at 0
```

### Expected code
```cpp
Vector2(T x1, T y1, T x2, T y2) : x(x2 - x1), y(y2 - y1) {}
```

### Impact
Any code that constructs a `Vector2` from two endpoints gets back `(0, 0)`.
Angle calculations on that zero vector lead to division by zero (see BUG-4).

---

## BUG-4 · HIGH — `Vector2::AngleBetween` divides by zero for zero-length vectors

**File:** `include/math/Vector2.hpp`  
**Function:** `Vector2<T>::AngleBetween`

### Description
The non-normalised branch divides by `lhs.len() * rhs.len()`.  If either
operand is the zero vector, `len()` returns `0.0f` and the division produces
**NaN** (or raises a floating-point exception depending on the platform).
The zero vector is an explicit sentinel in the system (`Vectors::ZERO`) and
can also arise from BUG-3 above.

### Buggy code
```cpp
static float AngleBetween(const Vector2 &lhs, const Vector2 &rhs) {
    if (lhs.isNormalized() && rhs.isNormalized()) {
        return std::acos(Vector2::Dot(lhs, rhs));
    } else {
        return std::acos(Vector2::Dot(lhs, rhs) / (lhs.len() * rhs.len())); // ← div by 0
    }
}
```

### Expected code
```cpp
static float AngleBetween(const Vector2 &lhs, const Vector2 &rhs) {
    float lenProduct = lhs.len() * rhs.len();
    if (Floats::eq(lenProduct, 0.0f)) {
        return 0.0f; // or std::numeric_limits<float>::quiet_NaN() – define a contract
    }
    if (lhs.isNormalized() && rhs.isNormalized()) {
        return std::acos(Vector2::Dot(lhs, rhs));
    }
    return std::acos(Vector2::Dot(lhs, rhs) / lenProduct);
}
```

### Impact
Passing any zero-length vector to `AngleBetween` produces NaN, which
propagates into the driving command (angle and speed become NaN), making the
servo and ESC commands undefined.

---

## BUG-5 · MEDIUM — `Vector2::normalized()` divides by zero for zero-length vectors

**File:** `include/math/Vector2.hpp`  
**Method:** `Vector2<T>::normalized()`

### Description
`normalized()` returns `Vector2(x / len(), y / len())`.  When called on a
zero vector, `len()` is `0.0f` and both components become `NaN` (or ±∞ on
some platforms).

### Buggy code
```cpp
Vector2 normalized() const { return Vector2(x / len(), y / len()); }
```

### Expected code
```cpp
Vector2 normalized() const {
    float l = len();
    if (Floats::eq(l, 0.0f)) { return *this; } // or return NULLVECT
    return Vector2(x / l, y / l);
}
```

### Impact
Lower severity than BUG-4 because `normalized()` is not called on the hot
path in the current algorithm; however, any future use on unvalidated input
will silently produce NaN components.

---

## BUG-6 · LOW — `scaleSpeedByCubeProximity` declared but never defined

**File:** `src/SeeingFinishLineSecondTimeImplFunctions.hpp`  
**Declaration:** `static speed_t scaleSpeedByCubeProximity(proximity_t proximity, speed_t maxSpeed);`

### Description
The function is declared `static` in the header but has no corresponding
definition anywhere in the project.  The compiler emits a warning
(`declared 'static' but never defined [-Wunused-function]`).  Any future
call to this function will produce a linker error.

### Impact
Dead code today; linker error if the function is ever called.

---

## Summary table

| ID | File | Severity | Short description |
|----|------|----------|-------------------|
| BUG-1 | `src/OnTrackState.cpp` | **CRITICAL** | 3+ vectors: wrong angle formula (raw vectors instead of avg vs NORTH) |
| BUG-2 | `include/params/speed.hpp` | **HIGH** | `scale(speed, proximity)`: integer division returns 0 for proximity 1–99 |
| BUG-3 | `include/math/Vector2.hpp` | **HIGH** | 4-arg constructor body is empty – always produces `(0, 0)` |
| BUG-4 | `include/math/Vector2.hpp` | **HIGH** | `AngleBetween` divides by zero for zero-length vectors |
| BUG-5 | `include/math/Vector2.hpp` | **MEDIUM** | `normalized()` divides by zero for zero-length vectors |
| BUG-6 | `src/SeeingFinishLineSecondTimeImplFunctions.hpp` | **LOW** | `scaleSpeedByCubeProximity` declared but never defined |
