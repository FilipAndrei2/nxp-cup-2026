#include "track_states/ATrackState.hpp"
#include "utils/lifesource.hpp"

size_t ls::ATrackState::numberFilteredVectors = 0;
ls::ATrackState::FilteredVectorsBuffer ls::ATrackState::filteredVectors;
bool ls::ATrackState::seeFinishLine = false;
bool ls::ATrackState::isInCrossway = false;
bool ls::ATrackState::cubeDetected = false;
ls::proximity_t ls::ATrackState::cubeProximity = 0;
