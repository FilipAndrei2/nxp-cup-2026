/**
 * @file main.cpp
 * @brief Punctul de intrare principal al aplicației pentru mașina NXP Cup.
 *
 * Inițializează drivere-le hardware și pornește bucla principală a mașinii.
 */
#include "utils/lifesource.hpp"
#include "utils/main_functions.hpp"

/**
 * @brief Funcția principală de intrare în program.
 *
 * Inițializează toate drivere-le hardware prin @c DriversInit(), construiește
 * obiectul @c Car folosind @c ls::buildCar() și pornește bucla de control
 * prin apelul @c run().
 *
 * @return Codul de ieșire returnat de bucla principală a mașinii (0 la succes).
 */
int main(void) {
  DriversInit();

  return ls::buildCar().run();
}
