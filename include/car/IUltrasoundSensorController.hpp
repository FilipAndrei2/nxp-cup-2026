#pragma once
#include "lifesource.hpp"

namespace ls {

    class IUltrasoundSensorController {
    public:
        // range: [0, 100]
        virtual uint8_t cubeProximity()        = 0;
        virtual ~IUltrasoundSensorController() = default;
    };
}
