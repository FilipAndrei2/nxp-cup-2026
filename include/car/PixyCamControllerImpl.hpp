#pragma once
#include "car/IPixyCamController.hpp"
#include "dp/TSingleton.hpp"

#include <memory>

namespace ls {
class PixyCamControllerImpl : public IPixyCamController,
                              public Singleton<PixyCamControllerImpl> {
  friend class Singleton<PixyCamControllerImpl>;

  /** @brief Returneaza vectorii de la camera pixy.
   * Vectorii returnati sunt normalizati (vectori directie).
   * @returns nullptr daca nu detecteaza niciun vector camera
   *
   */
  virtual std::shared_ptr<std::vector<FVector2>> getVectors() override;
  ~PixyCamControllerImpl() = default;

private:
  PixyCamControllerImpl();
};
} // namespace ls
