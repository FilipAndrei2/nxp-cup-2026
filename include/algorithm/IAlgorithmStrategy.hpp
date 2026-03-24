/**
 * @file IAlgorithmStrategy.hpp
 * @brief Definește interfața pentru strategia algoritmului de navigație.
 */
#pragma once
#include "dto/DrivingCommandDTO.hpp"
#include "dto/SensorDataDTO.hpp"
#include "utils/lifesource.hpp"

namespace ls {

/**
 * @brief Interfața pentru strategia algoritmului de navigație al mașinii.
 *
 * Această interfață definește contractul pe care orice implementare a
 * algoritmului de navigație trebuie să îl respecte. Primește datele de la
 * senzori și calculează parametrii de conducere corespunzători.
 */
class IAlgorithmStrategy {
public:
  /// @brief Destructor virtual implicit.
  virtual ~IAlgorithmStrategy() = default;

  /**
   * @brief Calculează parametrii de conducere pe baza datelor senzorilor.
   * @param sensorData Datele colectate de la senzorii mașinii.
   * @return Un obiect @c DrivingCommandDTO ce conține unghiul de direcție,
   *         viteza și dacă mașina trebuie oprită.
   */
  virtual const ls::DrivingCommandDTO
  computeParameters(const SensorDataDTO &sensorData) = 0;
};

} // namespace ls
