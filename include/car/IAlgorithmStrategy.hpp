#pragma once
#include <lifesource.hpp>
#include "dto/DrivingCommandDTO.hpp"
#include "dto/SensorDataDTO.hpp"

namespace ls {
    class IAlgorithmStrategy {
    public:
        virtual ~IAlgorithmStrategy() = default;
        virtual const ls::DrivingCommandDTO computeParameters(const std::unique_ptr<ls::SensorDataDTO> sensorData) = 0;
    };
}
