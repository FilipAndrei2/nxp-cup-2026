/* test/test_params.cpp
 * Tests for ls::Params – compile-time hardware constants.
 *
 * These tests verify that every constant in Params has the expected value.
 * They act as a regression guard: any inadvertent change to a tuning constant
 * will cause a test failure, prompting deliberate review.
 */
#include "catch2/catch_amalgamated.hpp"

#include "params/Params.hpp"

using namespace ls;

// ── Vector threshold ──────────────────────────────────────────────────────────

TEST_CASE("Params::FINISH_LINE_NUMBER_OF_VECTORS_THSH is 1", "[params]") {
    CHECK(Params::FINISH_LINE_NUMBER_OF_VECTORS_THSH == 1u);
}

// ── Cube proximity thresholds ─────────────────────────────────────────────────

TEST_CASE("Params::APPROACHING_CUBE_PERC_TSH is 20", "[params]") {
    CHECK(Params::APPROACHING_CUBE_PERC_TSH == 20u);
}

TEST_CASE("Params::STOP_CUBE_IS_TOO_CLOSE_TSH is 85", "[params]") {
    CHECK(Params::STOP_CUBE_IS_TOO_CLOSE_TSH == 85u);
}

TEST_CASE("Params – stop threshold is strictly greater than approach threshold", "[params]") {
    // Ensures the state machine can have a meaningful "approaching" window
    // before the car stops.
    CHECK(Params::STOP_CUBE_IS_TOO_CLOSE_TSH > Params::APPROACHING_CUBE_PERC_TSH);
}

// ── ESC duty cycles ───────────────────────────────────────────────────────────

TEST_CASE("Params::ESC_PWM_CHANNEL is 0", "[params]") {
    CHECK(Params::ESC_PWM_CHANNEL == 0u);
}

TEST_CASE("Params::ESC_MIN_DUTY_CYCLE is 1638", "[params]") {
    CHECK(Params::ESC_MIN_DUTY_CYCLE == 1638u);
}

TEST_CASE("Params::ESC_MED_DUTY_CYCLE is 2457", "[params]") {
    CHECK(Params::ESC_MED_DUTY_CYCLE == 2457u);
}

TEST_CASE("Params::ESC_MAX_DUTY_CYCLE is 3276", "[params]") {
    CHECK(Params::ESC_MAX_DUTY_CYCLE == 3276u);
}

TEST_CASE("Params – ESC duty cycles are strictly ordered MIN < MED < MAX", "[params]") {
    CHECK(Params::ESC_MIN_DUTY_CYCLE < Params::ESC_MED_DUTY_CYCLE);
    CHECK(Params::ESC_MED_DUTY_CYCLE < Params::ESC_MAX_DUTY_CYCLE);
}

// ── Servo duty cycles ─────────────────────────────────────────────────────────

TEST_CASE("Params::SERVO_PWM_CHANNEL is 0", "[params]") {
    CHECK(Params::SERVO_PWM_CHANNEL == 0u);
}

TEST_CASE("Params::SERVO_MIN_DUTY_CYCLE is 0", "[params]") {
    CHECK(Params::SERVO_MIN_DUTY_CYCLE == 0u);
}

TEST_CASE("Params::SERVO_MED_DUTY_CYCLE is 0", "[params]") {
    CHECK(Params::SERVO_MED_DUTY_CYCLE == 0u);
}

TEST_CASE("Params::SERVO_MAX_DUTY_CYCLE is 0", "[params]") {
    CHECK(Params::SERVO_MAX_DUTY_CYCLE == 0u);
}

// ── Pixy2 camera settings ─────────────────────────────────────────────────────

TEST_CASE("Params::PIXY2_I2C_CHANNEL is 0", "[params]") {
    CHECK(Params::PIXY2_I2C_CHANNEL == 0u);
}

TEST_CASE("Params::PIXY2CAM_FRAMERATE is 60", "[params]") {
    CHECK(Params::PIXY2CAM_FRAMERATE == 60u);
}
