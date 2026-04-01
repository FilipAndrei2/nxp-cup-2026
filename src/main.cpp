#include <utils/funcs.hpp>
#include "utils/lifesource.hpp"
#include "main_functions.h"
#include "params/Params.hpp"

#include "esc.h"
#include "pixy2.h"
#include "servo.h"

/**
 * Program entry point
 */
int main(void) {
	using namespace ls;
  DriversInit();
 



  return ls::buildCar().run();
}
