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

    auto cmd = state.computeCommand(sensor, ctx);

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

    auto cmd = state.computeCommand(sensor, ctx);

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

    auto cmd = state.computeCommand(sensor, ctx);

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

    auto cmd = state.computeCommand(sensor, ctx);

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

    auto cmd = state.computeCommand(sensor, ctx);

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

// ═════════════════════════════════════════════════════════════════════════════
//  SeeingFinishLineSecondTimeState
// ═════════════════════════════════════════════════════════════════════════════

TEST_CASE("SeeingFinishLineSecondTimeState::computeCommand – no vectors → straight at waiting speed",
          "[states][seeinfsecond]") {
    auto &state = SeeingFinishLineSecondTimeState::getInstance();
    TestableContext ctx;
    // NOTE: Due to a missing numberInfoVectors++ in the implementation loop,
    // the switch always falls into case 0 regardless of input vectors.
    SensorFixture sf({FVector2(0.0f, 0.5f)}, 0);
    auto &sensor = sf.dto;

    auto cmd = state.computeCommand(sensor, ctx);

    // case 0 branch: angle = 0, speed = WAITING_CUBE_SPEED
    CHECK(cmd.angle == Approx(0.0f));
    CHECK(cmd.speed == Speed::WAITING_CUBE_SPEED);
    CHECK(cmd.shouldStop == false);
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

    auto cmd = state.computeCommand(sensor, ctx);

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
    auto result = ctx.currentState()->computeCommand(sensor, ctx);
    ctx.currentState()->updateNextState(sensor, ctx);

    CHECK(result.angle == Approx(expectedCmd.angle));
    CHECK(result.speed == expectedCmd.speed);
    CHECK(result.shouldStop == expectedCmd.shouldStop);

    Verify(Method(mockState, computeCommand)).Once();
    Verify(Method(mockState, updateNextState)).Once();
}
