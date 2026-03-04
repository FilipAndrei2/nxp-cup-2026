#pragma once

namespace ls {
    class ICar {
    public: // Destructor virtual
        virtual ~ICar() = default;
    public:
        virtual int run() = 0;
    };
}