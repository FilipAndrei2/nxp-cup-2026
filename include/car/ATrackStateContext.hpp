#pragma once
#include "car/ITrackState.hpp"
#include "lifesource.hpp"

namespace ls {
class ITrackState;
class ATrackStateContext {

public:
  void setState(std::shared_ptr<ITrackState> state) { this->state = state; }

protected:
  ATrackStateContext() = default;

private:
  std::shared_ptr<ITrackState> state = nullptr;
};
} // namespace ls
