# Known Bugs

This document lists five critical bugs identified in the codebase. Each entry
describes the problem, the affected files and line numbers, and a suggested fix.

---

## Bug 1 — Dangling Reference: `Car::ctx` refers to a destroyed stack-local object

**Severity:** 🔴 Critical — undefined behaviour on every run  
**Affected files:**
- `include/car/Car.hpp` line 31
- `src/CarBuilder.cpp` lines 38–43
- `src/Car.cpp` line 19

### Description

`Car` stores its dependencies through a *reference* member:

```cpp
// include/car/Car.hpp  line 31
private:
  Car::Context &ctx;   // reference member
```

`CarBuilder::buildCar()` creates a `Context` as a **stack-local variable**,
passes it to the `Car` constructor (which binds the reference to that local),
and then returns. When the function returns the local `ctx` is destroyed, leaving
`Car::ctx` as a **dangling reference**. Every subsequent access inside
`Car::run()` (servo, ESC, sensor reads) is undefined behaviour.

```cpp
// src/CarBuilder.cpp  lines 38-43
Car CarBuilder::buildCar() const {
    auto ctx = Car::Context{...};  // ← stack-local, destroyed on return
    return Car(ctx);               // ← Car stores a reference to it
}                                  //   dangling after this point
```

### Suggested fix

Store `Context` by **value** inside `Car`, not by reference:

```cpp
// include/car/Car.hpp
private:
  Car::Context ctx;               // value member, not a reference

// src/Car.cpp
Car::Car(ls::Car::Context ctx) : ctx(std::move(ctx)) {}
```

---

## Bug 2 — `throw new std::runtime_error` — exception thrown as pointer, never caught

**Severity:** 🔴 Critical — unhandled exception terminates the program  
**Affected file:** `src/CarBuilder.cpp` line 35

### Description

```cpp
// src/CarBuilder.cpp  line 35
throw new std::runtime_error("Can't build car.");   // ❌ throws a pointer
```

`throw new T` allocates a `T` on the heap and throws a *pointer to it*.
Standard `catch (const std::runtime_error &)` clauses catch **values or
references**, not pointers, so this exception propagates uncaught and
terminates the process via `std::terminate`. The heap allocation is also never
freed, resulting in a memory leak.

### Suggested fix

Throw by value as is conventional in C++:

```cpp
throw std::runtime_error("Can't build car.");       // ✅ throw by value
```

---

## Bug 3 — Null pointer dereference: `getVectors()` returns `nullptr`, callers never check

**Severity:** 🔴 Critical — crash every time the camera sees no lines  
**Affected files:**
- `src/PixyCamControllerImpl.cpp` lines 17–19
- `src/OnTrackState.cpp` line 17
- `src/WaitingToApproachCubeState.cpp` line 15
- `src/SeeingFinishFirstTimeState.cpp` line 17
- `src/StartingBeforeFinishLineState.cpp` line 19
- `src/SeeingFinishLineSecondTimeState.cpp` lines 16, 85

### Description

When the Pixy2 camera detects no vectors, `getVectors()` returns `nullptr`:

```cpp
// src/PixyCamControllerImpl.cpp  lines 17-19
if (dv.NumberOfVectors == 0) {
    return nullptr;              // ❌ callers crash on dereference
}
```

Every caller dereferences `sensorData.vectors` without a null check:

| File | Line | Expression |
|------|------|-----------|
| `src/OnTrackState.cpp` | 17 | `filterTrackVectors(*sensorData.vectors, ...)` |
| `src/WaitingToApproachCubeState.cpp` | 15 | `sensorData.vectors->size()` |
| `src/SeeingFinishFirstTimeState.cpp` | 17 | `Vectors::notSeeingFinishLine(*sensorData.vectors)` |
| `src/StartingBeforeFinishLineState.cpp` | 19 | `Vectors::seeingFinishLine(*sensorData.vectors)` |
| `src/SeeingFinishLineSecondTimeState.cpp` | 16 | `filterInfoVectors(*sensorData.vectors, ...)` |
| `src/SeeingFinishLineSecondTimeState.cpp` | 85 | `Vectors::notSeeingFinishLine(*sensorData.vectors)` |

### Suggested fix

Return the already-allocated empty shared vector instead of `nullptr`:

```cpp
// src/PixyCamControllerImpl.cpp
if (dv.NumberOfVectors == 0) {
    return res;                  // ✅ return empty vector, never nullptr
}
```

---

## Bug 4 — Wrong steering angle in `OnTrackState` default case: `medi` is computed but not used

**Severity:** 🔴 Critical — incorrect steering whenever ≥ 3 track vectors are visible  
**Affected file:** `src/OnTrackState.cpp` lines 74–75

### Description

The `default` branch (three or more visible vectors) is supposed to average the
first two vectors and compute the angle between that average and `NORTH` — the
same algorithm used in `case 2`. However, `medi` is computed but then ignored:

```cpp
// src/OnTrackState.cpp  lines 74-75
default: {
    FVector2 medi = Vector2<float>::Avg(inInfoVectors[0], inInfoVectors[1]);
    // medi is computed ↑ but never used ↓
    outAngle = Vector2<float>::AngleBetween(inInfoVectors[0], inInfoVectors[1]);
    //                                      ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
    // ❌ angle between two raw input vectors, not between the average and NORTH
    outSpeed = Speed::scale(Speed::MAX, outAngle);
    break;
}
```

The `case 2` reference implementation (lines 65–67) correctly uses:
```cpp
outAngle = Vector2<float>::AngleBetween(medi, Vectors::NORTH);
```

### Suggested fix

```cpp
default: {
    FVector2 medi = Vector2<float>::Avg(inInfoVectors[0], inInfoVectors[1]);
    outAngle = Vector2<float>::AngleBetween(medi, Vectors::NORTH);  // ✅
    outSpeed = Speed::scale(Speed::MAX, outAngle);
    break;
}
```

---

## Bug 5 — Integer division in `Speed::scale(speed, proximity)` always produces zero

**Severity:** 🔴 Critical — cube-proximity speed scaling is completely non-functional  
**Affected files:**
- `include/params/speed.hpp` line 21
- `src/SeeingFinishLineSecondTimeState.cpp` line 78

### Description

`proximity_t` is defined as `uint32_t` (`include/utils/lifesource.hpp` line 20).
Both speed-scaling functions divide `proximity / 100` using **integer arithmetic**:

```cpp
// include/params/speed.hpp  line 21
static speed_t scale(const speed_t maxSpeed, const proximity_t cubeProxi) {
    return maxSpeed * (cubeProxi / 100);   // ❌ integer division; always 0
}

// src/SeeingFinishLineSecondTimeState.cpp  line 78
return maxSpeed * (proximity / 100);       // ❌ integer division; always 0
```

Because `cubeProxi` / `proximity` is a `uint32_t` value in the range [0, 100),
`cubeProxi / 100` truncates to **0** for every possible proximity value. The car
therefore always drives at full configured speed regardless of how close the cube
is, rendering the proximity-based slowdown completely inoperative.

### Suggested fix

Cast to `float` before dividing:

```cpp
// include/params/speed.hpp
static speed_t scale(const speed_t maxSpeed, const proximity_t cubeProxi) {
    return static_cast<speed_t>(maxSpeed * (cubeProxi / 100.0f));   // ✅
}

// src/SeeingFinishLineSecondTimeState.cpp
return static_cast<speed_t>(maxSpeed * (proximity / 100.0f));       // ✅
```
