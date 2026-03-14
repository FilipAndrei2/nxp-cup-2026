#include "utils/lifesource.hpp"
int main(void) {

  auto ctx = ls::createCarContext();

  return ls::Car(ctx).run();
}
