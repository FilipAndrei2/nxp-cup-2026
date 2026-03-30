# BUGS – fix/fix-bugs vs main

Code review comparing `fix/fix-bugs` against `main`.
Only `.cpp` / `.hpp` files were analysed (`.c` / `.h` are third-party middleware).

---

## BUG-1 – `src/ATrackState.cpp:4` — Wrong definition of static member `filteredVectors`

**Severity:** Critical (linker error)

```cpp
// Wrong – defines a global variable in the ls namespace,
// NOT the static member of ls::ATrackState
ls::ATrackState::FilteredVectorsBuffer filteredVectors;

// Correct
ls::ATrackState::FilteredVectorsBuffer ls::ATrackState::filteredVectors;
```

The class qualifier `ls::ATrackState::` is missing before the variable name.
As written, the line defines a new global variable called `filteredVectors` of type
`ls::ATrackState::FilteredVectorsBuffer`, while the actual static member
`ls::ATrackState::filteredVectors` remains undefined → **linker error** (undefined
reference to `ls::ATrackState::filteredVectors`).

---

## BUG-2 – `src/ATrackState.cpp` — Missing definition of static member `numberFilteredVectors`

**Severity:** Critical (linker error)

`include/track_states/ATrackState.hpp:85` declares:

```cpp
static size_t numberFilteredVectors;
```

`src/ATrackState.cpp` never provides a definition for this member.
Every translation unit that uses `ATrackState` will fail to link.

**Fix:** add to `src/ATrackState.cpp`:

```cpp
size_t ls::ATrackState::numberFilteredVectors = 0;
```

---

## BUG-3 – `src/BreakingState.cpp:13-17` — `computeAngle` and `computeSpeed` defined as free functions instead of class members

**Severity:** Critical (virtual override not applied)

```cpp
// Wrong – free functions in the ls namespace
angle_t computeAngle() { return (angle_t)0; }

speed_t computeSpeed(const angle_t inAngle, const proximity_t inCubeProxi) {
  return Speed::BREAK;
}
```

Both functions must be qualified with `BreakingState::`:

```cpp
// Correct
angle_t BreakingState::computeAngle() { return (angle_t)0; }

speed_t BreakingState::computeSpeed(const angle_t inAngle,
                                    const proximity_t inCubeProxi) {
  return Speed::BREAK;
}
```

Without the class qualifier the virtual functions declared in `BreakingState.hpp`
are never implemented, causing a link error or silently falling back to the
default/pure implementation in the base class.

---

## BUG-4 – `src/FinishedState.cpp:13` — Parameter name shadows the type name `proximity_t`

**Severity:** High (compilation error)

```cpp
speed_t FinishedState::computeSpeed(angle_t angle, proximity_t proximity_t) {
//                                                  ^^^^^^^^^^ ^^^^^^^^^^^
//                                                  type name  parameter name (same!)
```

Using the type name `proximity_t` as the parameter name is ill-formed and will
cause a compilation error on most compilers. The parameter must be renamed:

```cpp
speed_t FinishedState::computeSpeed(angle_t angle, proximity_t /*unused*/) {
  return (speed_t)0;
}
```

---

## BUG-5 – `src/SeeingFirstFinishState.cpp:23` — `computeSpeed` is a free function with an empty body

**Severity:** Critical (virtual override not applied + undefined behaviour)

```cpp
// Wrong – free function in the ls namespace, empty body (no return)
speed_t computeSpeed(const angle_t inAngle, const proximity_t inCubeProxi) {}
```

Two distinct problems:

1. **Missing class qualifier** – same issue as BUG-3.  
   The class qualifier `SeeingFirstFinishState::` is missing, so the virtual
   override declared in `SeeingFirstFinishState.hpp` is never implemented.

2. **Missing `return` statement** – a non-`void` function that falls off the end
   without returning a value is **undefined behaviour**.

**Fix:**

```cpp
speed_t SeeingFirstFinishState::computeSpeed(const angle_t inAngle,
                                              const proximity_t inCubeProxi) {
  return Speed::scale(Speed::MAX, inAngle);
}
```

---

## BUG-6 – `include/car/Car.hpp:31` + `src/Car.cpp:18` — Dangling reference in `Car`

**Severity:** Critical (undefined behaviour at runtime)

`Car.hpp` changed the member from a stored value to a reference:

```cpp
// Was (safe – value member)
Car::Context ctx;

// Now (dangerous – reference member)
Car::Context &ctx;
```

But the constructor still takes `Context` **by value**:

```cpp
Car::Car(ls::Car::Context ctx) : ctx(ctx) { … }
```

`ctx` in the initialiser list refers to the **local copy** of the parameter.
When the constructor returns, the local copy is destroyed, leaving `this->ctx` as
a **dangling reference**. Every subsequent access to `this->ctx` is undefined
behaviour (likely a segfault or data corruption).

**Fix:** take the constructor parameter by reference so that `this->ctx` is bound
to the caller's object, which must outlive the `Car` instance:

```cpp
Car::Car(ls::Car::Context &ctx) : ctx(ctx) { … }
```

and update the declaration in `Car.hpp` accordingly.

---

## BUG-7 – `include/params/speed.hpp:21-23` — `Speed::scale(maxSpeed, cubeProxi)` no longer divides by 100

**Severity:** High (wrong numerical result)

```cpp
// Old – correctly scales by percentage
return (speed_t)((float)maxSpeed * ((float)cubeProxi / 100.0f));

// New – multiplies by raw proximity value (e.g. 50 instead of 0.5)
return maxSpeed * cubeProxi;
```

`proximity_t` is a `uint32_t` percentage in the range `[0, 100]`.
Removing the `/100.0f` division means the result is 100× too large
(e.g. `scale(30, 50)` → `1500` instead of `15`).

**Fix:** restore the percentage division:

```cpp
static speed_t scale(const speed_t maxSpeed, const proximity_t cubeProxi) {
  return (speed_t)((float)maxSpeed * ((float)cubeProxi / 100.0f));
}
```

---

## BUG-8 – `test/Makefile:20-24` — References to deleted source files

**Severity:** High (test build broken)

The Makefile still lists three files that were removed in this branch:

```makefile
../src/SeeingFinishLineSecondTimeState.cpp   # deleted
../src/WaitingToApproachCubeState.cpp        # deleted
../src/StoppedState.cpp                      # deleted
```

These should be replaced with their new equivalents:

```makefile
../src/SeeingSecondFinishState.cpp
../src/WaitingCubeState.cpp
../src/BreakingState.cpp
../src/FinishedState.cpp
../src/ATrackState.cpp
```

---

## BUG-9 – `test/test_states.cpp` — Calls `updateNextState` with the old 2-parameter signature

**Severity:** High (test build broken)

The new `ITrackState` interface (changed in `include/track_states/ITrackState.hpp`)
defines:

```cpp
virtual void updateNextState(ATrackStateContext &ctx) const = 0;
```

However, every call-site in `test/test_states.cpp` still uses the old signature:

```cpp
state.updateNextState(sensor, ctx);  // old: (SensorDataDTO, ATrackStateContext)
```

This will fail to compile. All call-sites must be updated to:

```cpp
state.computeCommand(sensor);        // populate shared state first
state.updateNextState(ctx);
```

---

## BUG-10 – `test/test_states.cpp` — Includes headers that were deleted

**Severity:** High (test build broken)

The test file includes three headers that no longer exist:

```cpp
#include "track_states/SeeingFinishLineSecondTimeState.hpp"  // deleted
#include "track_states/StoppedState.hpp"                     // deleted
#include "track_states/WaitingToApproachCubeState.hpp"        // deleted
```

They should be replaced with the new equivalents:

```cpp
#include "track_states/SeeingSecondFinishState.hpp"
#include "track_states/WaitingCubeState.hpp"
#include "track_states/BreakingState.hpp"
#include "track_states/FinishedState.hpp"
```

---

## BUG-11 – `include/track_states/ATrackState.hpp:49-58` — `filterVectors` only collects finish-line vectors, breaking steering for all on-track states

**Severity:** High (wrong runtime behaviour)

```cpp
void filterVectors(const std::vector<ls::FVector2> &inRawVector) {
  clearBuffer();
  auto sawFinish = false;
  for (const auto &v : inRawVector) {
    if (Vectors::isFinishLine(v)) {   // ← only finish-line vectors are kept
      sawFinish = true;
      addFilteredVector(v);
    }
  }
  this->seeFinishLine = sawFinish;
}
```

The buffer populated by this function is then consumed by `computeAngle()` in the
base `ATrackState::computeCommand`. For states such as `OnTrackState` and
`WaitingCubeState`, the camera feeds **track** vectors (not finish-line vectors).
Because `filterVectors` discards every non-finish-line vector, the buffer will be
empty on ordinary track sections, `computeAngle()` returns `0` (straight ahead),
and the car loses all steering capability while following the track.

The original `OnTrackState` used a dedicated `filterTrackVectors` that did the
opposite (kept non-finish-line, non-zero vectors). The refactored base class should
either:

* keep both kinds of vectors and let subclasses specialise via a virtual method, or
* provide separate `filterTrackVectors` / `filterFinishVectors` helpers and call
  the appropriate one per state.

---

## BUG-12 – `include/params/Params.hpp:13` — Unused constant `SHOULD_STOP_CAR_CUBE_IS_CLOSE`

**Severity:** Low (dead code / unfinished implementation)

```cpp
static constexpr proximity_t SHOULD_STOP_CAR_CUBE_IS_CLOSE = 60;
```

This constant is defined but never referenced anywhere in the codebase.
It is likely a leftover from an incomplete refactor (the old threshold
`STOP_CUBE_IS_TOO_CLOSE_TSH = 85` was renamed but the new name was never
wired into any state transition). Either use it or remove it.
