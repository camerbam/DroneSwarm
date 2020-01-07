#ifndef DRONE_UPDATER_HPP
#define DRONE_UPDATER_HPP

#include "DroneSimulatorLib/State/DroneConfiguration.hpp"
#include "DroneSimulatorLib/State/DroneLocation.hpp"

namespace drone
{
  class DroneUpdater
  {
  public:
    DroneUpdater() {}

    virtual bool updateState(DroneLocation& location,
                             DroneConfiguration& config) = 0;
  };
} // namespace drone

#endif
