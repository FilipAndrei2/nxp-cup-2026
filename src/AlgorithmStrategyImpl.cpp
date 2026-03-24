/**
 * @file AlgorithmStrategyImpl.cpp
 * @brief Implementarea metodelor clasei @c AlgorithmStrategyImpl.
 */
#include "algorithm/AlgorithmStrategyImpl.hpp"

namespace ls {

/**
 * @brief Calculează parametrii de conducere delegând stării curente a pistei.
 *
 * Apelează @c computeCommand() pe starea curentă pentru a obține comanda
 * de conducere, apoi @c updateNextState() pentru a realiza tranziția la
 * starea următoare dacă este necesar.
 *
 * @param sensorData Datele colectate de la senzorii mașinii.
 * @return Comanda de conducere calculată de starea curentă.
 */
const ls::DrivingCommandDTO
AlgorithmStrategyImpl::computeParameters(const SensorDataDTO &sensorData) {
  auto res = this->getState().computeCommand(sensorData, *this);
  this->getState().updateNextState(sensorData, *this);

  return res;
}

} // namespace ls
