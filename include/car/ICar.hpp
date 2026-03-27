#pragma once

namespace ls {
    class ICar {
    public: // Destructor virtual
        virtual ~ICar() { }
    public:
        virtual int run() = 0;
    };
}
