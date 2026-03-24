#include "utils/lifesource.hpp"
#include "utils/main_functions.hpp"

/**
 * Program entry point
 */
int main(void) {
  DriversInit();

  return ls::buildCar().run();
}
