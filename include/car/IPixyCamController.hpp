#pragma once
#include "lifesource.hpp"
#include "math/Vector2.hpp"


namespace ls {
    class IPixyCam {
        virtual std::vector<FVector2> getVectors() = 0;
    };
}
