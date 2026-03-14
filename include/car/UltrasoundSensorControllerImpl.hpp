#pragma once
#include "car/IUltrasoundSensorController.hpp"
#include "dp/TSingleton.hpp"
#include "utils/lifesource.hpp"

namespace ls {
class UltrasoundSensorController
    : public IUltrasoundSensorController,
      public Singleton<UltrasoundSensorController> {
  friend class Singleton<UltrasoundSensorController>;

public:
  virtual uint8_t cubeProximity() override;
  virtual ~UltrasoundSensorController() = default;

private: // Ctor
  UltrasoundSensorController() = default;
};
} // namespace ls
