#pragma once
#include "lifesource.hpp"

#include <memory>

#include "ICar.hpp"
#include "IPixyCam.hpp"
#include "IServo.hpp"
#include "ISpeedController.hpp"
#include "IUltrasoundSensor.hpp"

namespace ls {
    class Car : public ICar {
        public: // Constructori
            Car() = delete; // Fara construcor de baza, parametri trebuiesc injectati pentru DI
            Car(std::unique_ptr<IPixyCam> pixyCam, 
                std::unique_ptr<IServo> servo, 
                std::unique_ptr<ISpeedController> speedController, 
                std::unique_ptr<IUltrasoundSensor> ultrasoundSensor); 
        
        public:
            virtual ~Car() {

            }   

        private:
            std::unique_ptr<IPixyCam> pixyCam;
            std::unique_ptr<IServo> servo;
            std::unique_ptr<ISpeedController> speedController;
            std::unique_ptr<IUltrasoundSensor> ultrasoundSensor;

            bool isRunning;
    }    
} // namespace ls

