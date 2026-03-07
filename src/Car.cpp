#include "lifesource.hpp"
#include "car/Car.hpp"

namespace ls {
    Car::Car(std::unique_ptr<IPixyCam> pixyCam, 
                std::unique_ptr<IServo> servo, 
                std::unique_ptr<ISpeedController> speedController, 
                std::unique_ptr<IUltrasoundSensor> ultrasoundSensor,
                std::unique_ptr<IAlgorithmStrategy> strat) {
            if (!pixyCam || !servo || !speedController || !ultrasoundSensor || !strat) {
                throw std::invalid_argument("ls::Car::Car(): invalid argument: nullptr");
            }

            this->m_pixyCam = std::move(pixyCam);
            this->m_servo = std::move(servo);
            this->m_speedController = std::move(speedController);
            this->m_ultrasoundSensor = std::move(ultrasoundSensor);
            this->m_strat          = std::move(strat);
            this->isRunning = true;
        }

    int Car::run() {
        // MainLoop 
        while (this->isRunning) {

        }

        return 0;
    }
}
