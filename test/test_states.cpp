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
#include "track_states/SeeingFirstFinishState.hpp"
#include "track_states/SeeingSecondFinishState.hpp"
#include "track_states/StartingState.hpp"
#include "track_states/BreakingState.hpp"
#include "track_states/FinishedState.hpp"
#include "track_states/WaitingCubeState.hpp"

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
//  StartingState
// ═════════════════════════════════════════════════════════════════════════════

TEST_CASE("StartingState::computeCommand – NORTH vector gives zero angle at max speed",
          "[states][starting]") {
    auto &state = StartingState::getInstance();
    TestableContext ctx;
    SensorFixture sf({FVector2(0.0f, 1.0f)});
    auto &sensor = sf.dto;

    auto cmd = state.computeCommand(sensor);

    CHECK(cmd.angle == Approx(0.0f));
    CHECK(cmd.speed == Speed::MAX);
    CHECK(cmd.shouldStop == false);
}

TEST_CASE("StartingState::updateNextState – transitions to SeeingFirstFinishState on finish line",
          "[states][starting]") {
    auto &state = StartingState::getInstance();
    TestableContext ctx;
    ctx.setState(&StartingState::getInstance());

    // A horizontal vector (y == 0) → finish line detected
    SensorFixture sf({FVector2(1.0f, 0.0f)});
    auto &sensor = sf.dto;
    state.computeCommand(sensor);
    state.updateNextState(ctx);

    CHECK(ctx.currentState() == &SeeingFirstFinishState::getInstance());
}

TEST_CASE("StartingState::updateNextState – stays put when no finish line",
          "[states][starting]") {
    auto &state = StartingState::getInstance();
    TestableContext ctx;
    ctx.setState(&StartingState::getInstance());

    SensorFixture sf({FVector2(0.0f, 0.5f)});
    auto &sensor = sf.dto;
    state.computeCommand(sensor);
    state.updateNextState(ctx);

    CHECK(ctx.currentState() == &StartingState::getInstance());
}

// ═════════════════════════════════════════════════════════════════════════════
//  SeeingFirstFinishState
// ═════════════════════════════════════════════════════════════════════════════

TEST_CASE("SeeingFirstFinishState::computeCommand – with finish-line vector, straight at waiting cube speed",
          "[states][seeinffirst]") {
    auto &state = SeeingFirstFinishState::getInstance();
    TestableContext ctx;
    SensorFixture sf({FVector2(1.0f, 0.0f)});
    auto &sensor = sf.dto;

    auto cmd = state.computeCommand(sensor);

    CHECK(cmd.angle == Approx(0.0f));
    CHECK(cmd.speed == Speed::WAITING_CUBE_SPEED);
    CHECK(cmd.shouldStop == false);
}

TEST_CASE("SeeingFirstFinishState::updateNextState – transitions to OnTrackState when finish line gone",
          "[states][seeinffirst]") {
    auto &state = SeeingFirstFinishState::getInstance();
    TestableContext ctx;
    ctx.setState(&SeeingFirstFinishState::getInstance());

    // No horizontal vectors → not seeing finish line
    SensorFixture sf({FVector2(0.0f, 0.5f)});
    auto &sensor = sf.dto;
    state.computeCommand(sensor);
    state.updateNextState(ctx);

    CHECK(ctx.currentState() == &OnTrackState::getInstance());
}

TEST_CASE("SeeingFirstFinishState::updateNextState – stays when finish line still visible",
          "[states][seeinffirst]") {
    auto &state = SeeingFirstFinishState::getInstance();
    TestableContext ctx;
    ctx.setState(&SeeingFirstFinishState::getInstance());

    SensorFixture sf({FVector2(1.0f, 0.0f)});
    auto &sensor = sf.dto;
    state.computeCommand(sensor);
    state.updateNextState(ctx);

    CHECK(ctx.currentState() == &SeeingFirstFinishState::getInstance());
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

TEST_CASE("OnTrackState::updateNextState – transitions to SeeingSecondFinishState",
          "[states][ontrack]") {
    auto &state = OnTrackState::getInstance();
    TestableContext ctx;
    ctx.setState(&OnTrackState::getInstance());

    SensorFixture sf({FVector2(1.0f, 0.0f)});
    auto &sensor = sf.dto;
    state.computeCommand(sensor);
    state.updateNextState(ctx);

    CHECK(ctx.currentState() == &SeeingSecondFinishState::getInstance());
}

TEST_CASE("OnTrackState::updateNextState – stays on track when no finish line",
          "[states][ontrack]") {
    auto &state = OnTrackState::getInstance();
    TestableContext ctx;
    ctx.setState(&OnTrackState::getInstance());

    SensorFixture sf({FVector2(0.0f, 0.5f)});
    auto &sensor = sf.dto;
    state.computeCommand(sensor);
    state.updateNextState(ctx);

    CHECK(ctx.currentState() == &OnTrackState::getInstance());
}

// ═════════════════════════════════════════════════════════════════════════════
//  SeeingSecondFinishState
// ═════════════════════════════════════════════════════════════════════════════

TEST_CASE("SeeingSecondFinishState::updateNextState – to WaitingCubeState when finish gone",
          "[states][seeinfsecond]") {
    auto &state = SeeingSecondFinishState::getInstance();
    TestableContext ctx;
    ctx.setState(&SeeingSecondFinishState::getInstance());

    SensorFixture sf({FVector2(0.0f, 0.5f)}, 0);
    auto &sensor = sf.dto;
    state.computeCommand(sensor);
    state.updateNextState(ctx);

    CHECK(ctx.currentState() == &WaitingCubeState::getInstance());
}

TEST_CASE("SeeingSecondFinishState::updateNextState – stays when finish line still visible",
          "[states][seeinfsecond]") {
    auto &state = SeeingSecondFinishState::getInstance();
    TestableContext ctx;
    ctx.setState(&SeeingSecondFinishState::getInstance());

    // Still seeing finish line
    SensorFixture sf({FVector2(1.0f, 0.0f)}, 0);
    auto &sensor = sf.dto;
    state.computeCommand(sensor);
    state.updateNextState(ctx);

    CHECK(ctx.currentState() == &SeeingSecondFinishState::getInstance());
}

// ═════════════════════════════════════════════════════════════════════════════
//  FinishedState
// ═════════════════════════════════════════════════════════════════════════════

TEST_CASE("FinishedState::computeCommand – always returns full stop",
          "[states][finished]") {
    auto &state = FinishedState::getInstance();
    TestableContext ctx;
    SensorFixture sf({FVector2(0.0f, 0.5f)}, 50);
    auto &sensor = sf.dto;

    auto cmd = state.computeCommand(sensor);

    CHECK(cmd.angle == Approx(0.0f));
    CHECK(cmd.speed == 0);
    CHECK(cmd.shouldStop == true);
}

TEST_CASE("FinishedState::updateNextState – terminal: state never changes",
          "[states][finished]") {
    auto &state = FinishedState::getInstance();
    TestableContext ctx;
    ctx.setState(&FinishedState::getInstance());

    SensorFixture sf({FVector2(1.0f, 0.0f)}, 100);
    auto &sensor = sf.dto;
    state.computeCommand(sensor);
    state.updateNextState(ctx);

    CHECK(ctx.currentState() == &FinishedState::getInstance());
}

// ═════════════════════════════════════════════════════════════════════════════
//  BreakingState
// ═════════════════════════════════════════════════════════════════════════════

TEST_CASE("BreakingState::computeCommand – always brakes (angle 0, negative speed, no stop)",
          "[states][breaking]") {
    auto &state = BreakingState::getInstance();
    TestableContext ctx;
    SensorFixture sf({FVector2(0.0f, 0.5f)}, 50);
    auto &sensor = sf.dto;

    auto cmd = state.computeCommand(sensor);

    CHECK(cmd.angle == Approx(0.0f));
    CHECK(cmd.speed == Speed::BREAK);
    CHECK(cmd.shouldStop == false);
}

TEST_CASE("BreakingState::updateNextState – transitions to FinishedState when cube very close",
          "[states][breaking]") {
    auto &state = BreakingState::getInstance();
    TestableContext ctx;
    ctx.setState(&BreakingState::getInstance());

    SensorFixture sf({}, Params::SHOULD_END_BREAK + 1);
    auto &sensor = sf.dto;
    state.computeCommand(sensor);
    state.updateNextState(ctx);

    CHECK(ctx.currentState() == &FinishedState::getInstance());
}

TEST_CASE("BreakingState::updateNextState – stays when cube not yet at end-break threshold",
          "[states][breaking]") {
    auto &state = BreakingState::getInstance();
    TestableContext ctx;
    ctx.setState(&BreakingState::getInstance());

    SensorFixture sf({}, Params::SHOULD_END_BREAK);
    auto &sensor = sf.dto;
    state.computeCommand(sensor);
    state.updateNextState(ctx);

    CHECK(ctx.currentState() == &BreakingState::getInstance());
}

// ═════════════════════════════════════════════════════════════════════════════
//  WaitingCubeState – transition to BreakingState
// ═════════════════════════════════════════════════════════════════════════════

TEST_CASE("WaitingCubeState::updateNextState – transitions to BreakingState when cube detected",
          "[states][waiting]") {
    auto &state = WaitingCubeState::getInstance();
    TestableContext ctx;
    ctx.setState(&WaitingCubeState::getInstance());

    SensorFixture sf({}, Params::APPROACHING_CUBE_PERC_TSH + 1);
    auto &sensor = sf.dto;
    state.computeCommand(sensor);
    state.updateNextState(ctx);

    CHECK(ctx.currentState() == &BreakingState::getInstance());
}

TEST_CASE("WaitingCubeState::updateNextState – stays when cube not yet detected",
          "[states][waiting]") {
    auto &state = WaitingCubeState::getInstance();
    TestableContext ctx;
    ctx.setState(&WaitingCubeState::getInstance());

    SensorFixture sf({}, Params::APPROACHING_CUBE_PERC_TSH);
    auto &sensor = sf.dto;
    state.computeCommand(sensor);
    state.updateNextState(ctx);

    CHECK(ctx.currentState() == &WaitingCubeState::getInstance());
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
    ctx.currentState()->updateNextState(ctx);

    CHECK(result.angle == Approx(expectedCmd.angle));
    CHECK(result.speed == expectedCmd.speed);
    CHECK(result.shouldStop == expectedCmd.shouldStop);

    Verify(Method(mockState, computeCommand)).Once();
    Verify(Method(mockState, updateNextState)).Once();
}

// ═════════════════════════════════════════════════════════════════════════════
//  WaitingCubeState – computeCommand
// ═════════════════════════════════════════════════════════════════════════════

TEST_CASE("WaitingCubeState::computeCommand – 0 vectors → straight at waiting cube speed",
          "[states][waiting]") {
    auto &state = WaitingCubeState::getInstance();
    TestableContext ctx;
    SensorFixture sf({}, 0);
    auto cmd = state.computeCommand(sf.dto);

    CHECK(cmd.angle == Approx(0.0f));
    CHECK(cmd.speed == Speed::WAITING_CUBE_SPEED);
    CHECK(cmd.shouldStop == false);
}

TEST_CASE("WaitingCubeState::computeCommand – NORTH vector → zero angle, waiting cube speed",
          "[states][waiting]") {
    auto &state = WaitingCubeState::getInstance();
    TestableContext ctx;
    // Vector (0,1) == Vectors::NORTH; AngleBetween(NORTH, Vectors::NORTH) = 0
    SensorFixture sf({FVector2(0.0f, 1.0f)}, 100);
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

TEST_CASE("SeeingSecondFinishState::computeCommand – non-finish vector at waiting cube speed",
          "[states][seeinfsecond]") {
    auto &state = SeeingSecondFinishState::getInstance();
    TestableContext ctx;
    SensorFixture sf({FVector2(0.0f, 0.5f)}, 100);
    auto cmd = state.computeCommand(sf.dto);

    CHECK(cmd.angle == Approx(0.0f));
    CHECK(cmd.speed == Speed::WAITING_CUBE_SPEED);
    CHECK(cmd.shouldStop == false);
}
