/* test/test_algorithm.cpp
 * Tests for ls::AlgorithmStrategyImpl – the top-level strategy that drives the
 * state machine.
 *
 * AlgorithmStrategyImpl::computeParameters:
 *   1. Calls getState().computeCommand(sensorData, *this)
 *   2. Calls getState().updateNextState(sensorData, *this)
 *   3. Returns the DrivingCommandDTO produced by computeCommand
 *
 * FakeIt is used to inject a mock ITrackState so we can verify the two calls
 * without relying on any hardware or real state implementations.
 */
#include "catch2/catch_amalgamated.hpp"
#include "fakeit/fakeit.hpp"

#include "algorithm/AlgorithmStrategyImpl.hpp"
#include "dto/DrivingCommandDTO.hpp"
#include "dto/SensorDataDTO.hpp"
#include "math/Vector2.hpp"
#include "track_states/ITrackState.hpp"

#include <memory>
#include <vector>

using namespace ls;
using namespace fakeit;
using Catch::Approx;

// ── Helper ────────────────────────────────────────────────────────────────────
// SensorDataDTO holds a REFERENCE to the shared_ptr, so the shared_ptr must
// outlive the DTO.  SensorFixture keeps them together.

struct SensorFixture {
    std::shared_ptr<std::vector<FVector2>> sp;
    SensorDataDTO dto;

    SensorFixture(std::vector<FVector2> vecs, proximity_t cube = 0)
        : sp(std::make_shared<std::vector<FVector2>>(std::move(vecs)))
        , dto{sp, cube}
    {}
};

// ═════════════════════════════════════════════════════════════════════════════
//  AlgorithmStrategyImpl – delegation via FakeIt mock
// ═════════════════════════════════════════════════════════════════════════════

TEST_CASE("AlgorithmStrategyImpl::computeParameters – calls computeCommand then updateNextState",
          "[algorithm][fakeit]") {
    Mock<ITrackState> mockState;
    DrivingCommandDTO fakeCmd{.angle = 0.75f, .speed = 50, .shouldStop = false};

    When(Method(mockState, computeCommand)).Return(fakeCmd);
    Fake(Method(mockState, updateNextState));

    auto &algo = AlgorithmStrategyImpl::getInstance();
    // Inject the mock state into the singleton context
    algo.setState(&mockState.get());

    SensorFixture sf({FVector2(0.0f, 1.0f)}, 0);
    auto &sensor = sf.dto;
    auto result = algo.computeParameters(sensor);

    // Verify the result is forwarded from computeCommand
    CHECK(result.angle == Approx(fakeCmd.angle));
    CHECK(result.speed == fakeCmd.speed);
    CHECK(result.shouldStop == fakeCmd.shouldStop);

    // Verify both lifecycle methods were called exactly once
    Verify(Method(mockState, computeCommand)).Once();
    Verify(Method(mockState, updateNextState)).Once();
}

TEST_CASE("AlgorithmStrategyImpl::computeParameters – shouldStop propagated correctly",
          "[algorithm][fakeit]") {
    Mock<ITrackState> mockState;
    DrivingCommandDTO stopCmd{.angle = 0.0f, .speed = 0, .shouldStop = true};

    When(Method(mockState, computeCommand)).Return(stopCmd);
    Fake(Method(mockState, updateNextState));

    auto &algo = AlgorithmStrategyImpl::getInstance();
    algo.setState(&mockState.get());

    SensorFixture sf({}, 100);
    auto &sensor = sf.dto;
    auto result = algo.computeParameters(sensor);

    CHECK(result.shouldStop == true);
    CHECK(result.speed == 0);
    CHECK(result.angle == Approx(0.0f));
}

TEST_CASE("AlgorithmStrategyImpl::computeParameters – updateNextState receives same sensor data",
          "[algorithm][fakeit]") {
    Mock<ITrackState> mockState;
    DrivingCommandDTO cmd{.angle = 0.0f, .speed = 100, .shouldStop = false};
    When(Method(mockState, computeCommand)).Return(cmd);

    // Capture the SensorDataDTO passed to updateNextState via a lambda
    proximity_t capturedProx = 999;
    When(Method(mockState, updateNextState))
        .Do([&](const SensorDataDTO &sd, ATrackStateContext &) {
            capturedProx = sd.cubeProximity;
        });

    auto &algo = AlgorithmStrategyImpl::getInstance();
    algo.setState(&mockState.get());

    SensorFixture sf({}, 42);
    auto &sensor = sf.dto;
    algo.computeParameters(sensor);

    CHECK(capturedProx == 42u);
}
