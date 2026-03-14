#include "algorithm/AlgorithmStrategyImpl.hpp"
#include "car/Car.hpp"
#include "math/Vector2.hpp"

int main(void) {
  ls::Car::Context ctx = {
      .engineController = ls::EngineControllerImpl::getInstance(),
      .algorithm = ls::AlgorithmStrategyImpl::getInstance()};
  return ls::Car(ctx).run();
}
