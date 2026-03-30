/* test/test_states.cpp
 * Tests for the track-state machine.
 *
 * Strategy:
 *  - Each concrete state (Singleton) is exercised directly via its
 *    getInstance() accessor.
 *  - State transitions are verified through a minimal TestableContext
 *    subclass that exposes the currently active state pointer.
 *  - FakeIt is used to mock ITrackState where the test requires verifying
 *    call interactions (e.g., the algorithm delegates correctly).
 */
#include "catch2/catch_amalgamated.hpp"
#include "fakeit/fakeit.hpp"

#include "dto/DrivingCommandDTO.hpp"
#include "dto/SensorDataDTO.hpp"
#include "math/Vectors.hpp"
#include "params/Params.hpp"
#include "params/speed.hpp"
#include "track_states/ATrackStateContext.hpp"
#include "track_states/ITrackState.hpp"
#include "track_states/OnTrackState.hpp"
#include "track_states/SeeingFinishFirstTimeState.hpp"
#include "track_states/SeeingFinishLineSecondTimeState.hpp"
#include "track_states/StartingBeforeFinishLineState.hpp"
#include "track_states/StoppedState.hpp"
#include "track_states/WaitingToAproachCubeState.hpp"

#include <memory>
#include <vector>

using namespace ls;
using namespace fakeit;
using Catch::Approx;

// ── Helper: expose state pointer from ATrackStateContext ──────────────────────

class TestableContext : public ATrackStateContext {
public:
    TestableContext() = default;
    ITrackState *currentState() { return &getState(); }
};

// ── Helper: build a SensorDataDTO ────────────────────────────────────────────
// SensorDataDTO stores a reference to the shared_ptr, so the shared_ptr must
// outlive the SensorDataDTO.  Use SensorFixture to keep both alive together.

struct SensorFixture {
    std::shared_ptr<std::vector<FVector2>> sp;
    SensorDataDTO dto;

    SensorFixture(std::vector<FVector2> vecs, proximity_t cubeProx = 0)
        : sp(std::make_shared<std::vector<FVector2>>(std::move(vecs)))
        , dto{sp, cubeProx}
    {}
};

// ═════════════════════════════════════════════════════════════════════════════
//  StartingBeforeFinishLineState
// ═════════════════════════════════════════════════════════════════════════════

TEST_CASE("StartingBeforeFinishLineState::computeCommand – always goes straight at max speed",
          "[states][starting]") {
    auto &state = StartingBeforeFinishLineState::getInstance();
    TestableContext ctx;
    SensorFixture sf({FVector2(1.0f, 0.5f)});
    auto &sensor = sf.dto;

    auto cmd = state.computeCommand(sensor);

    CHECK(cmd.angle == Approx(0.0f));
    CHECK(cmd.speed == Speed::MAX);
    CHECK(cmd.shouldStop == false);
}

TEST_CASE("StartingBeforeFinishLineState::updateNextState – transitions to SeeingFinishFirstState on finish line",
          "[states][starting]") {
    auto &state = StartingBeforeFinishLineState::getInstance();
    TestableContext ctx;
    ctx.setState(&StartingBeforeFinishLineState::getInstance());

    // A horizontal vector (y == 0) → finish line detected
    SensorFixture sf({FVector2(1.0f, 0.0f)});
    auto &sensor = sf.dto;
    state.updateNextState(sensor, ctx);

    CHECK(ctx.currentState() == &SeeingFinishFirstState::getInstance());
}

TEST_CASE("StartingBeforeFinishLineState::updateNextState – stays put when no finish line",
          "[states][starting]") {
    auto &state = StartingBeforeFinishLineState::getInstance();
    TestableContext ctx;
    ctx.setState(&StartingBeforeFinishLineState::getInstance());

    SensorFixture sf({FVector2(0.0f, 0.5f)});
    auto &sensor = sf.dto;
    state.updateNextState(sensor, ctx);

    CHECK(ctx.currentState() == &StartingBeforeFinishLineState::getInstance());
}

// ═════════════════════════════════════════════════════════════════════════════
//  SeeingFinishFirstState
// ═════════════════════════════════════════════════════════════════════════════

TEST_CASE("SeeingFinishFirstState::computeCommand – straight at max speed",
          "[states][seeinffirst]") {
    auto &state = SeeingFinishFirstState::getInstance();
    TestableContext ctx;
    SensorFixture sf({FVector2(1.0f, 0.0f)});
    auto &sensor = sf.dto;

    auto cmd = state.computeCommand(sensor);

    CHECK(cmd.angle == Approx(0.0f));
    CHECK(cmd.speed == Speed::MAX);
    CHECK(cmd.shouldStop == false);
}

TEST_CASE("SeeingFinishFirstState::updateNextState – transitions to OnTrackState when finish line gone",
          "[states][seeinffirst]") {
    auto &state = SeeingFinishFirstState::getInstance();
    TestableContext ctx;
    ctx.setState(&SeeingFinishFirstState::getInstance());

    // No horizontal vectors → not seeing finish line
    SensorFixture sf({FVector2(0.0f, 0.5f)});
    auto &sensor = sf.dto;
    state.updateNextState(sensor, ctx);

    CHECK(ctx.currentState() == &OnTrackState::getInstance());
}

TEST_CASE("SeeingFinishFirstState::updateNextState – stays when finish line still visible",
          "[states][seeinffirst]") {
    auto &state = SeeingFinishFirstState::getInstance();
    TestableContext ctx;
    ctx.setState(&SeeingFinishFirstState::getInstance());

    SensorFixture sf({FVector2(1.0f, 0.0f)});
    auto &sensor = sf.dto;
    state.updateNextState(sensor, ctx);

    CHECK(ctx.currentState() == &SeeingFinishFirstState::getInstance());
}

// ═════════════════════════════════════════════════════════════════════════════
//  OnTrackState
// ═════════════════════════════════════════════════════════════════════════════

TEST_CASE("OnTrackState::computeCommand – no (non-finish) vectors → straight at crossway speed",
          "[states][ontrack]") {
    auto &state = OnTrackState::getInstance();
    TestableContext ctx;
    // An empty vector list → numberInfoVectors == 0
    SensorFixture sf({});
    auto &sensor = sf.dto;

    auto cmd = state.computeCommand(sensor);

    CHECK(cmd.angle == Approx(0.0f));
    CHECK(cmd.speed == Speed::_4_WAY_CROSSWAY_SPEED);
    CHECK(cmd.shouldStop == false);
}

TEST_CASE("OnTrackState::computeCommand – one non-finish vector → angle from that vector",
          "[states][ontrack]") {
    auto &state = OnTrackState::getInstance();
    TestableContext ctx;
    // (0,1) == Vectors::NORTH: angle between NORTH and Vectors::NORTH = 0
    SensorFixture sf({FVector2(0.0f, 1.0f)});
    auto &sensor = sf.dto;

    auto cmd = state.computeCommand(sensor);

    // AngleBetween(NORTH, Vectors::NORTH) = 0 → speed = MAX * cos(0) = MAX
    CHECK(cmd.angle == Approx(0.0f).margin(1e-4f));
    CHECK(cmd.speed == Speed::scale(Speed::MAX, cmd.angle));
    CHECK(cmd.shouldStop == false);
}

TEST_CASE("OnTrackState::computeCommand – two non-finish vectors → average angle",
          "[states][ontrack]") {
    auto &state = OnTrackState::getInstance();
    TestableContext ctx;
    FVector2 v1(0.0f, 1.0f);  // NORTH direction
    FVector2 v2(0.0f, 1.0f);  // same
    SensorFixture sf({v1, v2});
    auto &sensor = sf.dto;

    auto cmd = state.computeCommand(sensor);

    CHECK(cmd.shouldStop == false);
    // Average of identical vectors is the same vector; angle with NORTH = 0
    CHECK(cmd.angle == Approx(0.0f).margin(1e-4f));
}

TEST_CASE("OnTrackState::updateNextState – transitions to SeeingFinishLineSecondTimeState",
          "[states][ontrack]") {
    auto &state = OnTrackState::getInstance();
    TestableContext ctx;
    ctx.setState(&OnTrackState::getInstance());

    SensorFixture sf({FVector2(1.0f, 0.0f)});
    auto &sensor = sf.dto;
    state.updateNextState(sensor, ctx);

    CHECK(ctx.currentState() == &SeeingFinishLineSecondTimeState::getInstance());
}

TEST_CASE("OnTrackState::updateNextState – stays on track when no finish line",
          "[states][ontrack]") {
    auto &state = OnTrackState::getInstance();
    TestableContext ctx;
    ctx.setState(&OnTrackState::getInstance());

    SensorFixture sf({FVector2(0.0f, 0.5f)});
    auto &sensor = sf.dto;
    state.updateNextState(sensor, ctx);

    CHECK(ctx.currentState() == &OnTrackState::getInstance());
}

TEST_CASE("SeeingFinishLineSecondTimeState::updateNextState – to WaitingToApproachCubeState when finish gone",
          "[states][seeinfsecond]") {
    auto &state = SeeingFinishLineSecondTimeState::getInstance();
    TestableContext ctx;
    ctx.setState(&SeeingFinishLineSecondTimeState::getInstance());

    SensorFixture sf({FVector2(0.0f, 0.5f)}, 0);
    auto &sensor = sf.dto;
    state.updateNextState(sensor, ctx);

    CHECK(ctx.currentState() == &WaitingToApproachCubeState::getInstance());
}

TEST_CASE("SeeingFinishLineSecondTimeState::updateNextState – to WaitingToApproachCubeState when cube detected",
          "[states][seeinfsecond]") {
    auto &state = SeeingFinishLineSecondTimeState::getInstance();
    TestableContext ctx;
    ctx.setState(&SeeingFinishLineSecondTimeState::getInstance());

    // cube proximity above threshold and still seeing finish line
    SensorFixture sf({FVector2(1.0f, 0.0f)},
                     Params::APPROACHING_CUBE_PERC_TSH + 1);
    auto &sensor = sf.dto;
    state.updateNextState(sensor, ctx);

    CHECK(ctx.currentState() == &WaitingToApproachCubeState::getInstance());
}

TEST_CASE("SeeingFinishLineSecondTimeState::updateNextState – stays when seeing finish, cube far",
          "[states][seeinfsecond]") {
    auto &state = SeeingFinishLineSecondTimeState::getInstance();
    TestableContext ctx;
    ctx.setState(&SeeingFinishLineSecondTimeState::getInstance());

    // Still seeing finish line, cube not close enough
    SensorFixture sf({FVector2(1.0f, 0.0f)}, 0);
    auto &sensor = sf.dto;
    state.updateNextState(sensor, ctx);

    CHECK(ctx.currentState() == &SeeingFinishLineSecondTimeState::getInstance());
}

// ═════════════════════════════════════════════════════════════════════════════
//  StoppedState
// ═════════════════════════════════════════════════════════════════════════════

TEST_CASE("StoppedState::computeCommand – always returns full stop",
          "[states][stopped]") {
    auto &state = StoppedState::getInstance();
    TestableContext ctx;
    SensorFixture sf({FVector2(0.0f, 0.5f)}, 50);
    auto &sensor = sf.dto;

    auto cmd = state.computeCommand(sensor);

    CHECK(cmd.angle == Approx(0.0f));
    CHECK(cmd.speed == 0);
    CHECK(cmd.shouldStop == true);
}

TEST_CASE("StoppedState::updateNextState – terminal: state never changes",
          "[states][stopped]") {
    auto &state = StoppedState::getInstance();
    TestableContext ctx;
    ctx.setState(&StoppedState::getInstance());

    SensorFixture sf({FVector2(1.0f, 0.0f)}, 100);
    auto &sensor = sf.dto;
    state.updateNextState(sensor, ctx);

    CHECK(ctx.currentState() == &StoppedState::getInstance());
}

// ═════════════════════════════════════════════════════════════════════════════
//  WaitingToApproachCubeState – transition to StoppedState
// ═════════════════════════════════════════════════════════════════════════════

TEST_CASE("WaitingToApproachCubeState::updateNextState – stops when cube too close",
          "[states][waiting]") {
    auto &state = WaitingToApproachCubeState::getInstance();
    TestableContext ctx;
    ctx.setState(&WaitingToApproachCubeState::getInstance());

    SensorFixture sf({}, Params::STOP_CUBE_IS_TOO_CLOSE_TSH);
    auto &sensor = sf.dto;
    state.updateNextState(sensor, ctx);

    CHECK(ctx.currentState() == &StoppedState::getInstance());
}

TEST_CASE("WaitingToApproachCubeState::updateNextState – stays when cube not yet too close",
          "[states][waiting]") {
    auto &state = WaitingToApproachCubeState::getInstance();
    TestableContext ctx;
    ctx.setState(&WaitingToApproachCubeState::getInstance());

    SensorFixture sf({}, Params::STOP_CUBE_IS_TOO_CLOSE_TSH - 1);
    auto &sensor = sf.dto;
    state.updateNextState(sensor, ctx);

    CHECK(ctx.currentState() == &WaitingToApproachCubeState::getInstance());
}

// ═════════════════════════════════════════════════════════════════════════════
//  FakeIt: mock ITrackState to verify ATrackStateContext delegation
// ═════════════════════════════════════════════════════════════════════════════

TEST_CASE("ATrackStateContext – delegates to the injected ITrackState mock",
          "[states][fakeit]") {
    Mock<ITrackState> mockState;
    TestableContext ctx;

    DrivingCommandDTO expectedCmd{.angle = 1.5f, .speed = 42, .shouldStop = false};

    // Set up expectations
    When(Method(mockState, computeCommand)).Return(expectedCmd);
    Fake(Method(mockState, updateNextState));

    ctx.setState(&mockState.get());

    SensorFixture sf({FVector2(0.0f, 1.0f)}, 0);
    auto &sensor = sf.dto;

    // Simulate what AlgorithmStrategyImpl::computeParameters does
    auto result = ctx.currentState()->computeCommand(sensor);
    ctx.currentState()->updateNextState(sensor, ctx);

    CHECK(result.angle == Approx(expectedCmd.angle));
    CHECK(result.speed == expectedCmd.speed);
    CHECK(result.shouldStop == expectedCmd.shouldStop);

    Verify(Method(mockState, computeCommand)).Once();
    Verify(Method(mockState, updateNextState)).Once();
}

// ═════════════════════════════════════════════════════════════════════════════
//  WaitingToApproachCubeState – computeCommand (previously untested)
// ═════════════════════════════════════════════════════════════════════════════

TEST_CASE("WaitingToApproachCubeState::computeCommand – 0 vectors, proximity 0 → speed 0",
          "[states][waiting]") {
    auto &state = WaitingToApproachCubeState::getInstance();
    TestableContext ctx;
    // 0 vectors → angle = 0, speed = scale(WAITING_CUBE_SPEED, 0.0f) = WAITING_CUBE_SPEED
    // then speed = scale(speed, 0) = speed * (0/100) = 0  (integer division)
    SensorFixture sf({}, 0);
    auto cmd = state.computeCommand(sf.dto);

    CHECK(cmd.angle == Approx(0.0f));
    CHECK(cmd.speed == 0);
    CHECK(cmd.shouldStop == false);
}

TEST_CASE("WaitingToApproachCubeState::computeCommand – 0 vectors, proximity 100 → full waiting speed",
          "[states][waiting]") {
    auto &state = WaitingToApproachCubeState::getInstance();
    TestableContext ctx;
    // 0 vectors → angle = 0, speed = scale(WAITING_CUBE_SPEED, 0.0f) = WAITING_CUBE_SPEED
    // then speed = scale(speed, 100) = speed * (100/100) = speed  (integer division)
    SensorFixture sf({}, 100);
    auto cmd = state.computeCommand(sf.dto);

    CHECK(cmd.angle == Approx(0.0f));
    CHECK(cmd.speed == Speed::WAITING_CUBE_SPEED);
    CHECK(cmd.shouldStop == false);
}

TEST_CASE("WaitingToApproachCubeState::computeCommand – 0 vectors, proximity 50 → speed 0",
          "[states][waiting]") {
    auto &state = WaitingToApproachCubeState::getInstance();
    TestableContext ctx;
    // cubeProxi / 100 is integer division: 50/100 = 0 → speed = 0
    SensorFixture sf({}, 50);
    auto cmd = state.computeCommand(sf.dto);

    CHECK(cmd.speed == 0);
    CHECK(cmd.shouldStop == false);
}

TEST_CASE("WaitingToApproachCubeState::computeCommand – 1 NORTH vector, proximity 100 → zero angle",
          "[states][waiting]") {
    auto &state = WaitingToApproachCubeState::getInstance();
    TestableContext ctx;
    // Vector (0,1) == Vectors::NORTH; AngleBetween(NORTH, Vectors::NORTH) = 0
    // speed = scale(WAITING_CUBE_SPEED, 0.0f) = WAITING_CUBE_SPEED
    // then scale(speed, 100) = WAITING_CUBE_SPEED
    SensorFixture sf({FVector2(0.0f, 1.0f)}, 100);
    auto cmd = state.computeCommand(sf.dto);

    CHECK(cmd.angle == Approx(0.0f).margin(1e-4f));
    CHECK(cmd.speed == Speed::WAITING_CUBE_SPEED);
    CHECK(cmd.shouldStop == false);
}

TEST_CASE("WaitingToApproachCubeState::computeCommand – 2 identical NORTH vectors, proximity 100",
          "[states][waiting]") {
    auto &state = WaitingToApproachCubeState::getInstance();
    TestableContext ctx;
    // Avg({0,1},{0,1}) = {0,1}, AngleBetween({0,1}, Vectors::NORTH) = 0
    SensorFixture sf({FVector2(0.0f, 1.0f), FVector2(0.0f, 1.0f)}, 100);
    auto cmd = state.computeCommand(sf.dto);

    CHECK(cmd.angle == Approx(0.0f).margin(1e-4f));
    CHECK(cmd.speed == Speed::WAITING_CUBE_SPEED);
    CHECK(cmd.shouldStop == false);
}

// ═════════════════════════════════════════════════════════════════════════════
//  OnTrackState – additional computeCommand cases
// ═════════════════════════════════════════════════════════════════════════════

TEST_CASE("OnTrackState::computeCommand – finish-line vectors are filtered out",
          "[states][ontrack]") {
    auto &state = OnTrackState::getInstance();
    TestableContext ctx;
    // (1,0) is a finish-line vector (|y|=0 <= 0.1); (0,1) is not
    // After filtering: numberInfoVectors = 1, infoVectors[0] = {0,1}
    SensorFixture sf({FVector2(1.0f, 0.0f), FVector2(0.0f, 1.0f)});
    auto cmd = state.computeCommand(sf.dto);

    // case 1: AngleBetween({0,1}, Vectors::NORTH) = 0
    CHECK(cmd.angle == Approx(0.0f).margin(1e-4f));
    CHECK(cmd.speed == Speed::MAX);
    CHECK(cmd.shouldStop == false);
}

TEST_CASE("OnTrackState::computeCommand – zero vectors are filtered out",
          "[states][ontrack]") {
    auto &state = OnTrackState::getInstance();
    TestableContext ctx;
    // {0,0} is filtered (v == Vectors::ZERO); {0,1} is kept
    SensorFixture sf({FVector2(0.0f, 0.0f), FVector2(0.0f, 1.0f)});
    auto cmd = state.computeCommand(sf.dto);

    CHECK(cmd.angle == Approx(0.0f).margin(1e-4f));
    CHECK(cmd.speed == Speed::MAX);
    CHECK(cmd.shouldStop == false);
}

TEST_CASE("OnTrackState::computeCommand – 3 non-finish vectors uses angle between first two",
          "[states][ontrack]") {
    auto &state = OnTrackState::getInstance();
    TestableContext ctx;
    // default branch: angle = AngleBetween(v0, v1) (not Avg→NORTH like case 2)
    // With all identical {0,1}: AngleBetween({0,1},{0,1}) = 0
    SensorFixture sf({FVector2(0.0f, 1.0f), FVector2(0.0f, 1.0f), FVector2(0.0f, 1.0f)});
    auto cmd = state.computeCommand(sf.dto);

    CHECK(cmd.angle == Approx(0.0f).margin(1e-4f));
    CHECK(cmd.speed == Speed::MAX);
    CHECK(cmd.shouldStop == false);
}

TEST_CASE("SeeingFinishLineSecondTimeState::computeCommand – proximity 100 preserves speed",
          "[states][seeinfsecond]") {
    auto &state = SeeingFinishLineSecondTimeState::getInstance();
    TestableContext ctx;
    // cubeProximity = 100 > 0 → speed = speed * (100/100) = speed * 1 = WAITING_CUBE_SPEED
    SensorFixture sf({FVector2(0.0f, 0.5f)}, 100);
    auto cmd = state.computeCommand(sf.dto);

    CHECK(cmd.angle == Approx(0.0f));
    CHECK(cmd.speed == Speed::WAITING_CUBE_SPEED);
    CHECK(cmd.shouldStop == false);
}

TEST_CASE("SeeingFinishLineSecondTimeState::computeCommand – proximity 50 zeroes speed via integer division",
          "[states][seeinfsecond]") {
    auto &state = SeeingFinishLineSecondTimeState::getInstance();
    TestableContext ctx;
    // cubeProximity = 50 > 0 → speed = speed * (50/100) = speed * 0 = 0  (integer division)
    SensorFixture sf({FVector2(0.0f, 0.5f)}, 50);
    auto cmd = state.computeCommand(sf.dto);

    CHECK(cmd.angle == Approx(0.0f));
    CHECK(cmd.speed == 0);
    CHECK(cmd.shouldStop == false);
}

// ═════════════════════════════════════════════════════════════════════════════
//  SeeingFinishLineSecondTimeState – additional computeCommand cases
// ═════════════════════════════════════════════════════════════════════════════

TEST_CASE("SeeingFinishLineSecondTimeState::computeCommand – 2 non-finish vectors, proximity 100",
          "[states][seeinfsecond]") {
    auto &state = SeeingFinishLineSecondTimeState::getInstance();
    TestableContext ctx;
    // 2 non-finish vectors (|y| > 0.1) → case 2+/default: avg({0,0.5},{0,0.5}) = {0,0.5}
    // AngleBetween({0,0.5}, NORTH) = 0 → speed = scale(WAITING_CUBE_SPEED, 0) = WAITING_CUBE_SPEED
    // Then scale(WAITING_CUBE_SPEED, 100) = WAITING_CUBE_SPEED (integer division: 100/100 = 1)
    SensorFixture sf({FVector2(0.0f, 0.5f), FVector2(0.0f, 0.5f)}, 100);
    auto cmd = state.computeCommand(sf.dto);

    CHECK(cmd.angle == Approx(0.0f).margin(1e-4f));
    CHECK(cmd.speed == Speed::WAITING_CUBE_SPEED);
    CHECK(cmd.shouldStop == false);
}

TEST_CASE("SeeingFinishLineSecondTimeState::computeCommand – finish-line vectors filtered out → 0 info vectors",
          "[states][seeinfsecond]") {
    auto &state = SeeingFinishLineSecondTimeState::getInstance();
    TestableContext ctx;
    // All input vectors are finish-line vectors (|y| <= 0.1) → filtered to 0 info vectors
    // case 0: angle = 0, speed = WAITING_CUBE_SPEED
    // Then scale(WAITING_CUBE_SPEED, 100) = WAITING_CUBE_SPEED
    SensorFixture sf({FVector2(1.0f, 0.0f), FVector2(2.0f, 0.05f)}, 100);
    auto cmd = state.computeCommand(sf.dto);

    CHECK(cmd.angle == Approx(0.0f));
    CHECK(cmd.speed == Speed::WAITING_CUBE_SPEED);
    CHECK(cmd.shouldStop == false);
}

TEST_CASE("SeeingFinishLineSecondTimeState::computeCommand – zero vector filtered out → 0 info vectors",
          "[states][seeinfsecond]") {
    auto &state = SeeingFinishLineSecondTimeState::getInstance();
    TestableContext ctx;
    // Zero vectors are filtered out by filterInfoVectors; result: 0 info vectors
    // case 0: angle = 0, speed = WAITING_CUBE_SPEED, then scale by proximity
    SensorFixture sf({FVector2(0.0f, 0.0f)}, 100);
    auto cmd = state.computeCommand(sf.dto);

    CHECK(cmd.angle == Approx(0.0f));
    CHECK(cmd.speed == Speed::WAITING_CUBE_SPEED);
    CHECK(cmd.shouldStop == false);
}

// ═════════════════════════════════════════════════════════════════════════════
//  OnTrackState – 4-vector boundary (max array capacity)
// ═════════════════════════════════════════════════════════════════════════════

TEST_CASE("OnTrackState::computeCommand – 4 non-finish vectors uses default branch (v0 vs v1 angle)",
          "[states][ontrack]") {
    auto &state = OnTrackState::getInstance();
    TestableContext ctx;
    // 4 vectors → default branch: angle = AngleBetween(v0, v1) (not avg→NORTH)
    // With identical NORTH vectors: AngleBetween({0,1},{0,1}) = 0
    SensorFixture sf({
        FVector2(0.0f, 1.0f),
        FVector2(0.0f, 1.0f),
        FVector2(0.0f, 1.0f),
        FVector2(0.0f, 1.0f),
    });
    auto cmd = state.computeCommand(sf.dto);

    CHECK(cmd.angle == Approx(0.0f).margin(1e-4f));
    CHECK(cmd.speed == Speed::MAX);
    CHECK(cmd.shouldStop == false);
}

// ═════════════════════════════════════════════════════════════════════════════
//  WaitingToApproachCubeState – 3+ vectors (default branch)
// ═════════════════════════════════════════════════════════════════════════════

TEST_CASE("WaitingToApproachCubeState::computeCommand – 3 vectors, proximity 100",
          "[states][waiting]") {
    auto &state = WaitingToApproachCubeState::getInstance();
    TestableContext ctx;
    // 3 vectors → default branch: Avg(v0, v1), AngleBetween(avg, NORTH)
    // With identical NORTH vectors: avg = {0,1}, angle = 0
    // speed = scale(WAITING_CUBE_SPEED, 0) = WAITING_CUBE_SPEED
    // Then scale(WAITING_CUBE_SPEED, 100) = WAITING_CUBE_SPEED
    SensorFixture sf({
        FVector2(0.0f, 1.0f),
        FVector2(0.0f, 1.0f),
        FVector2(0.0f, 1.0f),
    }, 100);
    auto cmd = state.computeCommand(sf.dto);

    CHECK(cmd.angle == Approx(0.0f).margin(1e-4f));
    CHECK(cmd.speed == Speed::WAITING_CUBE_SPEED);
    CHECK(cmd.shouldStop == false);
}

// ═════════════════════════════════════════════════════════════════════════════
//  ATrackStateContext – default initial state
// ═════════════════════════════════════════════════════════════════════════════

TEST_CASE("ATrackStateContext – default initial state is StartingBeforeFinishLineState",
          "[states][context]") {
    // A freshly constructed context must start in the initial state of the
    // race sequence without any explicit setState call.
    TestableContext ctx;
    CHECK(ctx.currentState() == &StartingBeforeFinishLineState::getInstance());
}
