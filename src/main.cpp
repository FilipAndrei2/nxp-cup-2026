#include <utils/funcs.hpp>
#include "utils/lifesource.hpp"
#include "main_functions.h"

/**
 * Program entry point
 */
int main(void) {
  DriversInit();

  return ls::buildCar().run();
}
