/**
 * @file main_functions.hpp
 * @brief Declarație pentru funcția de construire a mașinii.
 */
#pragma once
#include "car/Car.hpp"

namespace ls {

/**
 * @brief Construiește și returnează obiectul @c Car complet configurat.
 *
 * Instanțiază toate componentele singleton (cameră, servo, ESC, senzor
 * ultrasonic, algoritm) și le asamblează folosind @c CarBuilder.
 *
 * @return Un obiect @c Car gata de utilizare.
 */
Car buildCar();

} // namespace ls
