#pragma once

// includes
#include <memory>
#include <stdexcept>

#include <vector>

#include <cstdint>

#include "main_functions.h"

#define TODO() static_assert(true == false, "TODO: must impl method");

namespace ls {

// Tipuri de date
using angle_t = float;
using speed_t = int32_t;
using proximity_t = uint32_t;

} // namespace ls
