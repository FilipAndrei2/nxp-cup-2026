# BUGS.md — Code Review (branch: main)

> Scope: fișiere `.cpp` și `.hpp` din proiect (exclude `./test` și codul 3rd-party din `RTD/`).

---

## 🔴 CRITIC

### BUG-1 — `UltrasoundSensorController::cubeProximity()` returnează mereu 0
**Fișier:** `src/UltrasoundSensorController.cpp`

```cpp
uint8_t UltrasoundSensorController::cubeProximity() {
  return 0;   // todo:
}
```

Detectarea cubului este **complet nefuncțională**. Deoarece `cubeProximity()` returnează mereu 0:
- `cubeDetected` este mereu `false` (0 > `APPROACHING_CUBE_PERC_TSH` = 20 este false).
- `WaitingCubeState` nu tranzitionează niciodată spre `BreakingState`.
- Mașina nu va frâna și nu va termina cursa corect.

---

### BUG-2 — Condiție de ieșire din `BreakingState` pare inversată / nu se poate declanșa
**Fișier:** `src/BreakingState.cpp`, `include/params/Params.hpp`

```cpp
void BreakingState::updateNextState(ATrackStateContext &ctx) const {
  if (this->cubeProximity > Params::SHOULD_END_BREAK) {  // SHOULD_END_BREAK = 69
    ctx.setState(&FinishedState::getInstance());
  }
}
```

`Speed::BREAK = -100` înseamnă frânare maximă / mers invers, ceea ce **crește distanța față de cub** (proximitatea scade). Condiția `cubeProximity > 69` nu se va declanșa niciodată dacă mașina se îndepărtează de cub în timp ce frânează. Codul sugerează că logica ar trebui să fie că mașina se oprește atunci când cubul este suficient de aproape, dar din cauza BUG-1 și a semanticii BREAK, această tranziție este blocată.

---

### BUG-3 — `Params::SHOULD_STOP_CAR_CUBE_IS_CLOSE` definit dar niciodată utilizat
**Fișier:** `include/params/Params.hpp`

```cpp
static constexpr proximity_t SHOULD_STOP_CAR_CUBE_IS_CLOSE = 60;
```

Această constantă este definită, dar nu apare în niciun loc din cod. Numele ei sugerează că ar trebui să fie pragul de la care mașina trebuie să se oprească (analog cu `SHOULD_END_BREAK = 69`), indicând că implementarea frenării sau a detecției cubului este incompletă.

---

## 🟠 RIDICAT

### BUG-4 — `PixyCamControllerImpl::getVectors()` este implicit `private`
**Fișier:** `include/car/PixyCamControllerImpl.hpp`

```cpp
class PixyCamControllerImpl : public IPixyCamController,
                              public Singleton<PixyCamControllerImpl> {
  friend class Singleton<PixyCamControllerImpl>;

  // ← nicio specificare de acces; implicit private
  virtual std::shared_ptr<std::vector<Vector2<float>>> getVectors() override;
  ~PixyCamControllerImpl() = default;

private:
  PixyCamControllerImpl();
};
```

`getVectors()` (și destructorul) sunt în zona implicită `private` a clasei. Apelul prin polimorfism (pointer/referință la `IPixyCamController`) funcționează tehnic, dar apelul direct pe `PixyCamControllerImpl&` nu va compila. Lipsește `public:` înainte de metoda virtuală.

---

### BUG-5 — Variabilele statice din `ATrackState` sunt partajate între toate stările
**Fișier:** `include/track_states/ATrackState.hpp`, `src/ATrackState.cpp`

```cpp
static FilteredVectorsBuffer filteredVectors;
static size_t numberFilteredVectors;
static bool seeFinishLine;
static bool isInCrossway;
static bool cubeDetected;
static proximity_t cubeProximity;
```

Toate aceste câmpuri sunt `static`, deci sunt partajate de toate instanțele subclaselor (`OnTrackState`, `BreakingState`, etc.). Deși în practică se operează pe o singură stare la un moment dat, starea globală ascunsă poate genera bug-uri greu de diagnosticat la schimbarea stărilor. Câmpurile ar trebui să fie membri de instanță sau stocate explicit în contextul algoritmului.

---

### BUG-6 — Variabilă `static local` inutilă `medi` în `computeAngle()`
**Fișier:** `include/track_states/ATrackState.hpp`

```cpp
virtual angle_t computeAngle() {
  static Vector2<float> medi(0.0f, 0.0f);
  switch (numberFilteredVectors) {
    ...
  }
}
```

`medi` este `static local`, deci este inițializat o singură dată și persistă între apeluri. Nu există niciun motiv funcțional pentru care să fie static: valoarea este mereu suprascrisă înainte de a fi folosită în cazurile 2 și `default`. Utilizarea `static` locală introduce stare ascunsă și poate crea confuzie.

---

### BUG-7 — Constructorul cu 4 argumente al `Vector2` produce mereu `x ≥ 0` (quadrant greșit)
**Fișier:** `include/math/Vector2.hpp`

```cpp
Vector2(T x0, T y0, T x1, T y1) {
  this->x = x0 > x1 ? x0 - x1 : x1 - x0;  // mereu >= 0
  this->y = y0 > y1 ? y0 - y1 : y1 - y0;  // mereu >= 0
}
```

Comentariul spune că vectorul este „translatat în cadranele 1 și 2" (x poate fi negativ), dar implementarea returnează mereu `|x0-x1|` (valoare absolută), plasând vectorul exclusiv în cadranul 1. Turnele la stânga necesită `x < 0`, deci constructorul este incapabil să reprezinte direcții la stânga. Constructorul nu este utilizat în `PixyCamControllerImpl` (unde semnul lui `x` este calculat corect), dar prezența lui este înșelătoare și potențial periculoasă dacă va fi reutilizat.

---

### BUG-8 — `Speed::scale(speed_t, proximity_t)` definit dar niciodată apelat
**Fișier:** `include/params/speed.hpp`

```cpp
static speed_t scale(const speed_t maxSpeed, const proximity_t cubeProxi) {
  return (speed_t)((float)maxSpeed * ((float)cubeProxi / 100.0f));
}
```

Supraîncărcarea `scale` cu `proximity_t` nu este apelată nicăieri în cod. Aceasta este cod mort, posibil o funcție planificată pentru scalarea vitezei în funcție de proximitatea cubului, dar niciodată integrată.

---

## 🟡 MEDIU

### BUG-9 — Parametri neutilizați în `BreakingState::computeSpeed`
**Fișier:** `src/BreakingState.cpp`

```cpp
speed_t BreakingState::computeSpeed(const angle_t inAngle,
                                    const proximity_t inCubeProxi) {
  return Speed::BREAK;
}
```

`inAngle` și `inCubeProxi` nu sunt utilizați, ceea ce va genera avertismente de compilator (`-Wunused-parameter`). Parametrii ar trebui marcați cu `(void)` sau eliminați/înlocuiți cu `[[maybe_unused]]`.

---

### BUG-10 — Literal `double` în `constexpr float`
**Fișier:** `include/math/Vectors.hpp`

```cpp
static constexpr float HORIZONTAL_THRESHOLD = 0.1;   // 0.1 este double
```

`0.1` este un literal `double`, nu `float`. Ar trebui să fie `0.1f` pentru a evita conversia implicită și a exprima clar intenția.

---

### BUG-11 — Operatorul `operator=(Vector2&&)` nu zerorizează sursa
**Fișier:** `include/math/Vector2.hpp`

```cpp
Vector2(Vector2 &&other) : x(other.x), y(other.y) {
  other.x = (T)0;   // constructorul de mutare zerorizează sursa
  other.y = (T)0;
}

Vector2 &operator=(Vector2 &&other) {
  this->x = other.x;
  this->y = other.y;
  return *this;   // ← sursa nu este zerorizată
}
```

Constructorul de mutare zerorizează sursa, dar operatorul de atribuire prin mutare nu face același lucru. Comportament inconsistent.

---

### BUG-12 — Comentariu incorect/contradictoriu în `Vector2::AngleBetween`
**Fișier:** `include/math/Vector2.hpp`

```cpp
// Coordinate system: y-axis points up (math convention), so
//   positive result → lhs is clockwise from rhs  (right turn in screen space)
//   negative result → lhs is counter-clockwise   (left turn in screen space)
```

În convenția matematică (axa y în sus), un unghi pozitiv corespunde rotației **trigonometrice** (counter-clockwise). Comentariul afirmă opusul (`positive → clockwise`), creând o contradicție internă. Codul produce rezultate corecte în practică (conform utilizării în `ServoControllerImpl`), dar documentația este înșelătoare.

---

### BUG-13 — `throw std::runtime_error` în `CarBuilder::buildCar()` pe target embedded
**Fișier:** `src/CarBuilder.cpp`

```cpp
throw std::runtime_error("Can't build car.");
```

Pe un microcontroler ARM fără suport complet pentru excepții C++ (sau cu `-fno-exceptions`), aruncarea unei excepții poate produce comportament nedefinit, apel la `std::terminate`, sau creșterea semnificativă a dimensiunii binarului. Ar fi de preferat un mecanism de eroare compatibil cu sistemul embedded (ex. assert, cod de eroare, halt).

---

### BUG-14 — `Params::PIXY2CAM_FRAMERATE` definit dar niciodată utilizat
**Fișier:** `include/params/Params.hpp`

```cpp
static constexpr uint32_t PIXY2CAM_FRAMERATE = 60;
```

Constanta nu este utilizată nicăieri în cod. Dacă framerate-ul camerei nu este configurat programatic, această constantă este dead code.

---

## 🔵 MINOR

### BUG-15 — `ServoController.hpp` este un fișier gol
**Fișier:** `include/car/ServoController.hpp`

Fișierul există dar nu conține nicio declarație utilă (doar linii goale). Poate fi un rest dintr-un refactor, generând confuzie.

---

### BUG-16 — `IPixyCamController::changeLedColor` este cod mort și fără validare a intervalului
**Fișier:** `include/car/IPixyCamController.hpp`

```cpp
static void changeLedColor(const int r, const int g, const int b) {
  Pixy2SetLed(r, g, b);
}
```

Funcția nu este apelată nicăieri. În plus, parametrii sunt `int` fără restricționare la `[0, 255]`, putând transmite valori negative sau prea mari la driverul hardware.

---

### BUG-17 — `TODO()` macro definit cu `static_assert(true == false, ...)` în loc de `static_assert(false, ...)`
**Fișier:** `include/utils/lifesource.hpp`

```cpp
#define TODO() static_assert(true == false, "TODO: must impl method");
```

Macro-ul funcționează corect (va produce eroare de compilare dacă este instanțiat), dar `static_assert(true == false, ...)` este o formulare inutilă. Forma canonică este `static_assert(false, ...)`. De asemenea, macro-ul nu este utilizat nicăieri în cod.

---

### BUG-18 — `PixyCamControllerImpl::getVectors()` — rezervare ineficientă a vectorului
**Fișier:** `src/PixyCamControllerImpl.cpp`

```cpp
res->reserve(dv.NumberOfVectors / 2);
```

Dacă `dv.NumberOfVectors` este 1, `1 / 2 = 0` (divizare întreagă), deci nu se rezervă spațiu. Comentariul care justifică `/ 2` nu este riguros matematic. O valoare fixă mică (ex. `res->reserve(4)`) sau `res->reserve(dv.NumberOfVectors)` ar fi mai clară și mai corectă.

---

*Generat la: 2026-03-31 | Revizie: branch main*
