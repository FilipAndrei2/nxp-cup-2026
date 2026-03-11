#pragma once
#include "ICar.hpp"
#include "IPixyCamController.hpp"
#include "IServoController.hpp"
#include "ISpeedController.hpp"
#include "IUltrasoundSensorController.hpp"
#include "algorithm/IAlgorithmStrategy.hpp"
#include "utils/lifesource.hpp"

namespace ls {
class Car : public ICar {
public:
  struct Context {
    IPixyCamController &pixySensor;
    IServoController &servoController;
    ISpeedController &engineController;
    IUltrasoundSensorController &ultrasoundSensor;
    IAlgorithmStrategy &algorithm;
  };

public:           // Constructori
  Car() = delete; // Fara construcor de baza, parametri trebuiesc injectati
                  // pentru DI
  Car(ls::Car::Context &ctx);

public:
  virtual int run(void) override;
  virtual ~Car() = default;

private:
  Car::Context &ctx;
  bool isRunning;

private:
  void stopCar();
  SensorDataDTO readSensors();
  DrivingCommandDTO computeDrivingCommand(const SensorDataDTO &sensorData);
  void controlCar(const DrivingCommandDTO drivingCommand);
};
} // namespace ls
