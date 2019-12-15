#ifndef LOCATION_UPDATER_HPP
#define LOCATION_UPDATER_HPP

#include <atomic>

#include "DroneSimulatorLib/State/DroneLocation.hpp"
#include "DroneUpdater.hpp"

namespace drone
{
  class LocationUpdater : public DroneUpdater
  {
  public:
    LocationUpdater(const DroneLocation& targetLocation);

    bool updateState(DroneLocation& location,
                     DroneConfiguration& config) override;

  private:
    DroneLocation m_targetLocation;
  };
} // namespace drone

#endif
