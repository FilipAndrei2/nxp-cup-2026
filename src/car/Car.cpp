#include "lifesource.hpp"
#include "Car.hpp"

namespace ls {
    Car::Car(std::unique_ptr<IPixyCam> pixyCam, 
                std::unique_ptr<IServo> servo, 
                std::unique_ptr<ISpeedController> speedController, 
                std::unique_ptr<IUltrasoundSensor> ultrasoundSensor) 
        {
            if (!pixyCam || !servo || !speedController || !ultrasoundSensor) {
                throw std::invalid_argument("ls::Car::Car(): invalid argument: nullptr");
            }

            this->pixyCam = std::move(pixyCam)
            this->servo = std::move(servo);
            this->speedController = std::move(speedController);
            this->ultrasoundSensor = std::move(ultrasoundSensor);
            this->isRunning = true;
        }

    int Car::run() {
        // MainLoop
        while (this->isRunning) {

        }

        return 0;
    }
}