#pragma once
#include "math/Vector2.hpp"
#include "math/Vectors.hpp"
#include "track_states/ITrackState.hpp"
#include <array>

namespace ls {

/**
 * Fiecare state trebuie sa overrideze viteza maxima obligatoriu
 * 
 */
class ATrackState : public ITrackState {

public:
  virtual const ls::DrivingCommandDTO
  computeCommand(const ls::SensorDataDTO &sensorData) override {
    filterVectors(*sensorData.vectors);
    detectCrossway();
    detectCube(sensorData.cubeProximity);

    auto steer = computeSteer();
    auto speed = computeSpeed(steer, sensorData.cubeProximity);

    return DrivingCommandDTO {
        .steer = steer, .speed = speed, .shouldStop = shouldStopCar()
    };
  }

  static constexpr size_t FV_BUFF_SIZE = 5;
  using FilteredVectorsBuffer = std::array<FVector2, FV_BUFF_SIZE>;

protected:
  void clearBuffer() {
    this->numberFilteredVectors = 0;
    seeFinishLine = false;
  }

  /**
   * Adds a vector to the buffer
   * If buffer capacity is exceeded, does not add the vector to the buffer
   */
  void addFilteredVector(const FVector2 &fv) {
    if (numberFilteredVectors >= filteredVectors.max_size()) {
      return;
    }
    filteredVectors[numberFilteredVectors].setX(fv.getX());
    filteredVectors[numberFilteredVectors].setY(fv.getY());
    numberFilteredVectors++;
  }

  void filterVectors(const std::vector<ls::FVector2> &inRawVector) {
    clearBuffer();
    auto sawFinish = false;
    for (const auto &v : inRawVector) {
      if (Vectors::isFinishLine(v)) {
        sawFinish = true;
      } else {
        addFilteredVector(v);
      }
    }
    this->seeFinishLine = sawFinish;
  }

  virtual int computeSteer() {
    static Vector2<float> medi(0.0f, 0.0f);
    switch (numberFilteredVectors) {
    case 0: {
      return (angle_t)0;
    }
    case 1: {
      return Vector2<float>::AngleToSteer(filteredVectors[0]);
    }
    default: {
      medi = Vector2<float>::Avg(filteredVectors[0], filteredVectors[1]);
      return Vector2<float>::AngleToSteer(medi);
    }
    }
  }

  virtual bool shouldStopCar() { return false; }

  virtual speed_t MAX_SPEED(void) const = 0;


  virtual speed_t computeSpeed(int steer, const proximity_t inCubeProxi) {
    if (steer < 0) {
      steer = -steer;
    }
    return this->MAX_SPEED() * (speed_t)steer;
  }

  static FilteredVectorsBuffer filteredVectors;
  static size_t numberFilteredVectors;
  static bool seeFinishLine;
  static bool isInCrossway;
  static bool cubeDetected;
  static proximity_t cubeProximity; // Needed for detecting when to stop break


private:
  void detectCrossway() {
    this->isInCrossway =
        this->numberFilteredVectors == 0 && this->seeFinishLine == false;
  }

  void detectCube(const proximity_t cubeProximity) {
    ATrackState::cubeProximity =
        cubeProximity; // Needed for detecting when to stop break.
    this->cubeDetected = cubeProximity > Params::APPROACHING_CUBE_PERC_TSH;
  }
};
} // namespace ls
