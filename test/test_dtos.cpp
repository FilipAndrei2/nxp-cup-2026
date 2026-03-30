/* test/test_dtos.cpp
 * Tests for ls::DrivingCommandDTO and ls::SensorDataDTO.
 *
 * Both are plain aggregate structs.  The tests verify that every field can be
 * set and read back correctly, including boundary / special values.
 */
#include "catch2/catch_amalgamated.hpp"

#include "dto/DrivingCommandDTO.hpp"
#include "dto/SensorDataDTO.hpp"
#include "math/Vector2.hpp"

#include <memory>
#include <vector>

using namespace ls;
using Catch::Approx;

// ═════════════════════════════════════════════════════════════════════════════
//  DrivingCommandDTO
// ═════════════════════════════════════════════════════════════════════════════

TEST_CASE("DrivingCommandDTO – normal forward command", "[dto][driving]") {
    DrivingCommandDTO cmd{.angle = 0.0f, .speed = 100, .shouldStop = false};
    CHECK(cmd.angle == Approx(0.0f));
    CHECK(cmd.speed == 100);
    CHECK(cmd.shouldStop == false);
}

TEST_CASE("DrivingCommandDTO – stop command", "[dto][driving]") {
    DrivingCommandDTO cmd{.angle = 0.0f, .speed = 0, .shouldStop = true};
    CHECK(cmd.angle == Approx(0.0f));
    CHECK(cmd.speed == 0);
    CHECK(cmd.shouldStop == true);
}

TEST_CASE("DrivingCommandDTO – angle and speed are independent", "[dto][driving]") {
    DrivingCommandDTO cmd{.angle = 1.5f, .speed = 42, .shouldStop = false};
    CHECK(cmd.angle == Approx(1.5f));
    CHECK(cmd.speed == 42);
    CHECK(cmd.shouldStop == false);
}

TEST_CASE("DrivingCommandDTO – negative angle is preserved", "[dto][driving]") {
    DrivingCommandDTO cmd{.angle = -0.785f, .speed = 70, .shouldStop = false};
    CHECK(cmd.angle == Approx(-0.785f));
    CHECK(cmd.speed == 70);
    CHECK(cmd.shouldStop == false);
}

TEST_CASE("DrivingCommandDTO – zero speed with non-zero angle", "[dto][driving]") {
    DrivingCommandDTO cmd{.angle = 1.0f, .speed = 0, .shouldStop = false};
    CHECK(cmd.angle == Approx(1.0f));
    CHECK(cmd.speed == 0);
    CHECK(cmd.shouldStop == false);
}

TEST_CASE("DrivingCommandDTO – shouldStop true with non-zero angle and speed", "[dto][driving]") {
    // Edge case: shouldStop can be set independently of angle/speed values.
    DrivingCommandDTO cmd{.angle = 0.5f, .speed = 50, .shouldStop = true};
    CHECK(cmd.shouldStop == true);
    CHECK(cmd.angle == Approx(0.5f));
    CHECK(cmd.speed == 50);
}

// ═════════════════════════════════════════════════════════════════════════════
//  SensorDataDTO
// ═════════════════════════════════════════════════════════════════════════════

TEST_CASE("SensorDataDTO – empty vectors list, zero cube proximity", "[dto][sensor]") {
    auto sp = std::make_shared<std::vector<FVector2>>();
    SensorDataDTO dto{sp, 0u};

    CHECK(dto.vectors != nullptr);
    CHECK(dto.vectors->empty());
    CHECK(dto.cubeProximity == 0u);
}

TEST_CASE("SensorDataDTO – non-empty vectors list accessible", "[dto][sensor]") {
    auto sp = std::make_shared<std::vector<FVector2>>(
        std::vector<FVector2>{FVector2(1.0f, 0.0f), FVector2(0.0f, 1.0f)});
    SensorDataDTO dto{sp, 50u};

    REQUIRE(dto.vectors->size() == 2u);
    CHECK((*dto.vectors)[0].getX() == Approx(1.0f));
    CHECK((*dto.vectors)[0].getY() == Approx(0.0f));
    CHECK((*dto.vectors)[1].getX() == Approx(0.0f));
    CHECK((*dto.vectors)[1].getY() == Approx(1.0f));
}

TEST_CASE("SensorDataDTO – cubeProximity 100 (maximum)", "[dto][sensor]") {
    auto sp = std::make_shared<std::vector<FVector2>>();
    SensorDataDTO dto{sp, 100u};
    CHECK(dto.cubeProximity == 100u);
}

TEST_CASE("SensorDataDTO – cubeProximity propagated to DTO correctly", "[dto][sensor]") {
    auto sp = std::make_shared<std::vector<FVector2>>();
    SensorDataDTO dto{sp, 37u};
    CHECK(dto.cubeProximity == 37u);
}

TEST_CASE("SensorDataDTO – vectors shared_ptr is the same object that was passed", "[dto][sensor]") {
    auto sp = std::make_shared<std::vector<FVector2>>();
    SensorDataDTO dto{sp, 0u};
    // The shared_ptr stored in the DTO must alias the same control block.
    CHECK(dto.vectors.get() == sp.get());
}
