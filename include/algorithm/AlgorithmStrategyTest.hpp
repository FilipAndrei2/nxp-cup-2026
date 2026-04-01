#pragma once
#include "dp/TSingleton.hpp"
#include "algorithm/AlgorithmStrategyImpl.hpp"
#include "track_states/TestState.hpp"
#include "track_states/ATrackStateContext.hpp"
#include "algorithm/IAlgorithmStrategy.hpp"

namespace ls {
class AlgorithmStrategyTest : public ATrackStateContext, public IAlgorithmStrategy, public Singleton<AlgorithmStrategyTest> {
friend class Singleton<AlgorithmStrategyTest>;

public:
virtual ~AlgorithmStrategyTest() { }

virtual const ls::DrivingCommandDTO
computeParameters(const SensorDataDTO &sensorData) {
	return this->getState().computeCommand(sensorData);
}

AlgorithmStrategyTest () {
		this->setState(&TestState::getInstance());
	}
};
}
