#pragma once
#include "lifesource.hpp"

#include <memory>

#include "math/Vector2.hpp"

namespace ls {

    struct SensorDataDTO {
        std::unique_ptr<std:vector<ls::Vector2<float>>> vectors;
        ls::proximity_t cubeProximity;
    };
}
