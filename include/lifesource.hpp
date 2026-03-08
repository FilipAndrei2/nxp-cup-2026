#pragma once

// includes
#include <stdexcept>
#include <memory>

#include <vector>

#include <cstdint>

#define TODO() \
static_assert(true == false, "TODO: must impl method");

namespace ls {

    // Tipuri de date
    using angle_t = float;
    using speed_t = uint32_t;
    using proximity_t = uint32_t;

    // DIVERSI PARAMETRI HARDWARE
    constexpr uint32_t PIXY2CAM_FRAMERATE = 60; // todo: VERIFICA CORECTITUDINEA
}
