#ifndef ANGLE_UPDATER_HPP
#define ANGLE_UPDATER_HPP

#include <atomic>

#include "DroneSimulatorLib/State/DroneLocation.hpp"
#include "DroneUpdater.hpp"

namespace drone
{
  class AngleUpdater : public DroneUpdater
  {
  public:
    AngleUpdater(const double& targetAngle);

    bool updateState(DroneLocation& location,
                     DroneConfiguration& config) override;

  private:
    double m_targetAngle;
  };
} // namespace drone

#endif
