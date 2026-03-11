#pragma once
#include "utils/lifesource.hpp"

namespace ls {

    class ISpeedController {
    public:
        virtual void changeSpeed(ls::speed_t speed) = 0;
        virtual ~ISpeedController() = default;
    };
}
