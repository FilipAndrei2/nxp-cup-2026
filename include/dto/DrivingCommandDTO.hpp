/**
 * @file DrivingCommandDTO.hpp
 * @brief Definește structura de transfer a comenzii de conducere.
 */
#pragma once
#include "utils/lifesource.hpp"

namespace ls {

/**
 * @brief DTO (Data Transfer Object) pentru comanda de conducere a mașinii.
 *
 * Conține toți parametrii necesari pentru a controla direcția și viteza
 * mașinii, precum și un indicator de oprire.
 */
struct DrivingCommandDTO {
  angle_t angle;    ///< Unghiul de direcție al roților, exprimat în radiani.
  speed_t speed;    ///< Viteza dorită în intervalul [-100, 100].
  bool shouldStop;  ///< Dacă este @c true, mașina trebuie oprită imediat.
};

} // namespace ls
