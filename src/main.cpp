#include "algorithm/AlgorithmStrategyImpl.hpp"
#include "car/Car.hpp"
#include "math/Vector2.hpp"

int main(void) {
  ls::Car::Context ctx = {.algorithm = ls::AlgorithmStrategyImpl()};
  return ls::Car(ctx).run();
}
