#pragma once

namespace ls {

    class IServoController {
    public:
        virtual void steer(ls::angle_t angle) = 0;
        virtual ~IServoController() = 0;
    };
}
