#pragma once
#include "lifesource.hpp"
#include "Vector2.hpp"

class IPixyCam {
    virtual std::vector<FVector2> getVectors() = 0;
}