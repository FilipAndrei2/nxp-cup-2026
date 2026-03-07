#pragma once
#include "lifesource.hpp"

namespace ls {
    struct DrivingCommandDTO {
        angle_t angle;
        speed_t speed;
        bool    shouldStop;
    };
}
