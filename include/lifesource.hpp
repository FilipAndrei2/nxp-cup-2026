#pragma once

// includes
#include <stdexcept>

#define TODO() \
static_assert(true == false, "TODO: must impl method");
