#include "algorithm/AlgorithmStrategyImpl.hpp"
#include "algorithm/AlgorithmStrategyTest.hpp"
#include "car/Car.hpp"
#include "car/CarBuilder.hpp"
#include "car/PixyCamControllerImpl.hpp"
#include "car/ServoControllerImpl.hpp"
#include "car/SpeedControllerImpl.hpp"
#include "car/UltrasoundSensorControllerImpl.hpp"

namespace ls {
ls::Car buildCar() {
    auto builder = ls::CarBuilder()
        .setPixyCamController(ls::PixyCamControllerImpl::getInstance())
        .setSpeedController(ls::SpeedControllerImpl::getInstance())
        .setServoController(ls::ServoControllerImpl::getInstance())
        .setUltrasoundSensorController(ls::UltrasoundSensorController::getInstance())

#ifdef LS_DEBUG
        .setAlgorithmStrat(ls::AlgorithmStrategyTest::getInstance());
        ls::AlgorithmStrategyTest::getInstance().setInitialState();
#else 
        .setAlgorithmStrat(ls::AlgorithmStrategyImpl::getInstance());
        ls::AlgorithmStrategyImpl::getInstance().setInitialState();
#endif // LS_DEBUG
    return builder.buildCar();
}

void delay(size_t ticks) {
    if (ticks == 0) {
        return;
    }
    volatile size_t i = ticks;
    while (i > 0) {
        i--;
    }
}
}
