#pragma once
#include "track_states/OnTrackState.hpp"
#include "track_states/ATrackStateContext.hpp"
#include "params/speed.hpp"

namespace ls {
	class TestState : public OnTrackState {
	public:
		virtual void updateNextState(ATrackStateContext& context) const override;
	protected:
		virtual int MAX_SPEED() { return Speed::TEST_SPEED; }
};
}
