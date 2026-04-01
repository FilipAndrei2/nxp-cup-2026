#pragma once
#include "track_states/OnTrackState.hpp"
#include "track_states/ATrackStateContext.hpp"

namespace ls {
	class TestState : public OnTrackState {
	public:
		virtual void updateNextState(ATrackStateContext& context) const override final;
	};
}
