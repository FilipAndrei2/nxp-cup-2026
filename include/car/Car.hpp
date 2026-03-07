#pragma once
#include "lifesource.hpp"

#include <memory>

#include "car/ICar.hpp"
#include "car/IPixyCamController.hpp"
#include "car/IServoController.hpp"
#include "car/ISpeedController.hpp"
#include "car/IUltrasoundSensorController.hpp"
#include "car/IAlgorithmStrategy.hpp"

namespace ls {
    class Car : public ICar {
        public: // Constructori
            Car() = delete; // Fara construcor de baza, parametri trebuiesc injectati pentru DI
            Car(std::unique_ptr<IPixyCamController> pixyCam, 
                std::unique_ptr<IServoController> servo, 
                std::unique_ptr<ISpeedControllerController> speedController, 
                std::unique_ptr<IUltrasoundSensorController> ultrasoundSensor,
                std::unique_ptr<IAlgorithmStrategy> strat
                ); 
        
        public:
            virtual ~Car() = default;

        private:
            std::unique_ptr<IPixyCamController>             m_pixyCam;
            std::unique_ptr<IServoController>               m_servo;
            std::unique_ptr<ISpeedController>               m_speedController;
            std::unique_ptr<IUltrasoundSensorController>    m_ultrasoundSensor;
            std::unique_ptr<IAlgorithmStrategy>             m_strat;

            bool isRunning;
    };    
} // namespace ls

