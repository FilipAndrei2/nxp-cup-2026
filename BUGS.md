# Bug Report – NXP Cup 2026

Code review of all `*.cpp` and `*.hpp` files in the project (3rd-party firmware excluded).
Bugs are listed in descending order of severity.

---

## CRITICAL

### BUG-1 · `throw new` in `CarBuilder::buildCar()` – exception thrown as a heap pointer

**File:** `src/CarBuilder.cpp`

```cpp
// BEFORE (buggy)
throw new std::runtime_error("Can't build car.");

// AFTER (fixed)
throw std::runtime_error("Can't build car.");
```

`throw new T(...)` throws a `T*` pointer. Standard `catch (std::exception &)` and
`catch (std::runtime_error &)` clauses do **not** catch a pointer type, so the
exception propagates unhandled and terminates the process. The heap allocation is
also never freed, causing a memory leak.

**Fix applied:** `src/CarBuilder.cpp` – removed `new`.

---

### BUG-2 · `Car` stores `Context` by dangling reference

**Files:** `include/car/Car.hpp`, `src/Car.cpp`

```cpp
// BEFORE (buggy) – reference member
private:
  Car::Context &ctx;   // ← lvalue reference

Car::Car(ls::Car::Context &ctx) : ctx(std::move(ctx)) { ... }
//                                     ^ std::move on a reference is a no-op here
```

`CarBuilder::buildCar()` creates `ctx` as a local variable and then constructs
`Car(ctx)`. The `Car` object stores a *reference* to that local. When
`buildCar()` returns the `Car` by value the local `ctx` is destroyed, leaving
`Car::ctx` as a dangling reference. Any subsequent call to `Car::run()` is
undefined behaviour (use-after-free).

Additionally, `std::move` applied to a non-const lvalue reference member
initialiser is ill-formed for a reference member: the initialiser
`std::move(ctx)` produces an rvalue, which cannot bind to a non-const lvalue
reference member (`Car::Context &ctx`).

**Fix applied:** `include/car/Car.hpp` and `src/Car.cpp` – `Car::Context` is now
stored **by value**.

---

### BUG-3 · `vectors` used in `Car::readSensors()` without being declared

**Files:** `include/car/Car.hpp`, `src/Car.cpp`

```cpp
// src/Car.cpp
SensorDataDTO Car::readSensors() {
  vectors = this->ctx.pixySensor.getVectors();  // ← 'vectors' not declared
  auto cubeProximity = this->ctx.ultrasoundSensor.cubeProximity();
  return {vectors, cubeProximity};
}
```

`vectors` is neither a local variable nor a member of `Car` – there is no
`vectors` field in `Car.hpp`. This is a compile error on any target build that
includes `Car.cpp`.

**Fix applied:** `include/car/Car.hpp` – added
`std::shared_ptr<std::vector<FVector2>> vectors;` as a private member.

---

## HIGH

### BUG-4 · `Vector2::AngleBetween` always returns a non-negative angle

**File:** `include/math/Vector2.hpp`

```cpp
// BEFORE (buggy)
static float AngleBetween(const Vector2 &lhs, const Vector2 &rhs) {
  if (lhs.isNormalized() && rhs.isNormalized())
    return std::acos(Vector2::Dot(lhs, rhs));
  else
    return std::acos(Vector2::Dot(lhs, rhs) / (lhs.len() * rhs.len()));
}
```

`std::acos` always returns a value in `[0, π]`. The sign of the angle (left
turn vs. right turn) is therefore always lost. `Angles::angleToDir` maps
negative angles to left steering and positive angles to right steering. With
this bug, the car **can never steer left** regardless of the track vector
direction.

**Fix applied:** `include/math/Vector2.hpp` – use the sign of
`Vector2::Cross(lhs, rhs)` to determine the turn direction and negate the angle
when the vector is to the left of the reference. Also added a dot-product clamp
to `[-1, 1]` to prevent `acos` domain errors caused by floating-point rounding
(see BUG-7).

**Test cases added:** `test/test_vector2.cpp`
- `AngleBetween – right-of-reference gives positive angle`
- `AngleBetween – left-of-reference gives negative angle`
- `AngleBetween – diagonal right gives positive angle`
- `AngleBetween – diagonal left gives negative angle`

---

### BUG-5 · `PixyCamControllerImpl::getVectors()` discards the sign of the x component

**File:** `src/PixyCamControllerImpl.cpp`

```cpp
// BEFORE (buggy) – x and y are uint16_t, always non-negative
uint16_t x, y;
if (v.y0 < v.y1) {
  y = v.y1 - v.y0;
  x = (v.x0 < v.x1) ? v.x1 - v.x0 : v.x0 - v.x1;   // always |Δx|
} else {
  y = v.y0 - v.y1;
  x = (v.x0 < v.x1) ? v.x1 - v.x0 : v.x0 - v.x1;   // always |Δx|
}
res->emplace_back((float)x, (float)y);
```

Using `uint16_t` for `x` forces the result to be the absolute difference `|x1 -
x0|`, discarding whether the line segment goes left or right. After
normalisation, every vector ends up with `x ≥ 0` (first quadrant only). Combined
with BUG-4 (unsigned `AngleBetween`), the car can never detect a left-turn
track vector and will always steer right or straight.

**Fix applied:** `src/PixyCamControllerImpl.cpp` – switched to `int16_t dx, dy`
so the x-direction sign is preserved. The y component is kept positive (forward)
by flipping both components when `y0 < y1`.

---

## MEDIUM

### BUG-6 · `Vector2` 4-argument constructor has an empty body (UB – uninitialised members)

**File:** `include/math/Vector2.hpp`

```cpp
// BEFORE (buggy) – body is empty; x and y are never initialised
Vector2(T x1, T y1, T x2, T y2) {}
```

The doc-comment says this constructor creates the vector from two endpoints by
translating the segment to the origin, but the body is empty. For non-class
types (`float`, `int`, …) `x` and `y` are left with indeterminate values,
making any use of the resulting object **undefined behaviour**.

**Fix applied:** `include/math/Vector2.hpp`

```cpp
Vector2(T x1, T y1, T x2, T y2) : x(x2 - x1), y(y2 - y1) {}
```

**Test cases added:** `test/test_vector2.cpp`
- `4-argument constructor creates direction vector from two endpoints`
- `4-argument constructor with coincident points gives zero vector`

---

### BUG-7 · `Vector2::AngleBetween` can produce NaN for zero-length vectors

**File:** `include/math/Vector2.hpp`

When either input vector has length zero the non-normalised branch computes
`dot / (0 * len)` → division by zero, then `std::acos(NaN/Inf)` → NaN. A NaN
angle propagates into the servo command and produces undefined hardware
behaviour.

**Fix applied:** Combined with the BUG-4 fix in `include/math/Vector2.hpp` –
zero-length vectors now return `0.0f` early, and the dot product is clamped to
`[-1, 1]` before the `acos` call.

**Test cases added:** `test/test_vector2.cpp`
- `AngleBetween – zero-length lhs returns 0 (no NaN)`
- `AngleBetween – anti-parallel unit vectors return PI (no NaN)`

---

### BUG-8 · `WaitingToApproachCubeState::computeCommand` does not filter finish-line / zero vectors

**File:** `src/WaitingToApproachCubeState.cpp`

```cpp
// BEFORE (buggy) – uses raw vector count, no filtering
switch (sensorData.vectors->size()) {
case 1:
  angle = AngleBetween((*sensorData.vectors)[0], Vectors::NORTH);
  ...
}
```

All other states (`OnTrackState`, `SeeingFinishLineSecondTimeState`) filter out
finish-line vectors (`|y| ≤ 0.1`) and zero vectors before computing the steering
angle. `WaitingToApproachCubeState` skips this step, so a spurious finish-line
vector (which is nearly horizontal) can cause a large, incorrect steering angle
near the stopping point.

**Fix applied:** `src/WaitingToApproachCubeState.cpp` – added the same
finish-line and zero-vector filter used by the other states.

**Test cases added:** `test/test_states.cpp`
- `WaitingToApproachCubeState::computeCommand – finish-line vector is filtered out`
- `WaitingToApproachCubeState::computeCommand – zero vector is filtered out`
- `WaitingToApproachCubeState::computeCommand – only finish-line vectors gives straight ahead`

---

## LOW

### BUG-9 · Dead function declaration `scaleSpeedByCubeProximity` never defined

**File:** `src/SeeingFinishLineSecondTimeImplFunctions.hpp`

```cpp
// Declared but never defined or called; produces compiler warning
static speed_t scaleSpeedByCubeProximity(proximity_t proximity, speed_t maxSpeed);
```

GCC warns: *'ls::speed_t ls::scaleSpeedByCubeProximity(proximity_t, speed_t)'
declared 'static' but never defined.* The function is never called; it is dead
code left over from a refactoring.

**Fix applied:** Removed the declaration from
`src/SeeingFinishLineSecondTimeImplFunctions.hpp`.

---

### BUG-10 · `ServoControllerImpl.hpp` includes `utils/lifesource.hpp` twice

**File:** `include/car/ServoControllerImpl.hpp`

```cpp
#pragma once
#include "car/IServoController.hpp"
#include "utils/lifesource.hpp"
#include <dp/TSingleton.hpp>
#include "utils/lifesource.hpp"   // ← duplicate
```

The redundant include is harmless because `#pragma once` guards against multiple
processing, but it is dead code and creates confusion.

**Fix applied:** Removed the duplicate `#include "utils/lifesource.hpp"`.

---

## Notes

- **Speed scaling (integer division in `Speed::scale(speed_t, proximity_t)`)** –
  acknowledged and intentionally excluded from this report per project owner
  instruction.
- **`ServoController.hpp`** – the file is empty. It appears to be a placeholder
  that was never filled in and can be removed when convenient.
- **`IClock.hpp`** – defines an `IClock` interface that is never implemented or
  used anywhere in the codebase. It can be removed when convenient.
