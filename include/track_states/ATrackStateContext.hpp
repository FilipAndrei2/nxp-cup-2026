/**
 * @file ATrackStateContext.hpp
 * @brief Definește clasa de bază abstractă pentru contextul stărilor pistei.
 */
#pragma once
#include "track_states/ITrackState.hpp"
#include "track_states/StartingBeforeFinishLineState.hpp"
#include "utils/lifesource.hpp"

namespace ls {
class ITrackState;

/**
 * @brief Clasa de bază abstractă pentru contextul pattern-ului State al pistei.
 *
 * Stochează și gestionează starea curentă a pistei. Starea inițială
 * este @c StartingBeforeFinishLineState. Clasele care moștenesc această
 * clasă pot schimba starea curentă prin metoda @c setState().
 */
class ATrackStateContext {

public:
  /**
   * @brief Setează starea curentă a pistei.
   * @param state Pointer la noua stare de activat.
   */
  void setState(ITrackState *state) { this->state = state; }

protected:
  /// @brief Constructor protejat implicit.
  ATrackStateContext() = default;

  /**
   * @brief Returnează referința la starea curentă a pistei.
   * @return Referință la obiectul @c ITrackState curent.
   */
  ITrackState &getState() { return *state; }

private:
  /// @brief Pointer la starea curentă; inițializată cu @c StartingBeforeFinishLineState.
  ITrackState *state = &StartingBeforeFinishLineState::getInstance();
};

} // namespace ls
