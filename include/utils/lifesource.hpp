/**
 * @file lifesource.hpp
 * @brief Include-uri comune și definiții de tipuri utilizate în întregul proiect.
 *
 * Acest fișier acționează ca un header centralizat de bază. Definește
 * tipurile de date specifice domeniului (unghiuri, viteze, proximitate)
 * și macrocomanda @c TODO() pentru metodele neimplementate.
 */
#pragma once

// includes
#include <memory>
#include <stdexcept>

#include <vector>

#include <cstdint>

#include "main_functions.h"

/// @brief Macrocomandă care generează o eroare de compilare pentru metodele neimplementate.
#define TODO() static_assert(true == false, "TODO: must impl method");

namespace ls {

/// @brief Tipul pentru unghiuri, exprimat în radiani (număr în virgulă mobilă).
using angle_t = float;

/// @brief Tipul pentru viteză, exprimat în intervalul [-100, 100].
using speed_t = int32_t;

/// @brief Tipul pentru proximitate, exprimat procentual în intervalul [0, 100].
using proximity_t = uint32_t;

} // namespace ls
