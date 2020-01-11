
#include "LocationUpdater.hpp"

#include "DroneSimulatorLib/DroneSimulatorConsts.hpp"
#include "RegistryLib/Registry.hpp"

namespace
{
  const size_t SPEED_RATIO(5);
  double updateDimension(double ratio,
                         double current,
                         double target,
                         size_t speed)
  {
    double diff(ratio * SPEED_RATIO * speed / drone::FRAMES_PER_SECOND *
                GlobalRegistry::getRegistry().getSpeedRatio());
    if (current < target)
    {
      if (current + diff > target)
        return target;
      else
        return current + diff;
    }
    if (current - diff > target)
      return current - diff;
    else
      return target;
  }

  bool compareDoubles(const double& a, const double& b)
  {
    return std::abs(a - b) < .00001;
  }
}

drone::LocationUpdater::LocationUpdater(const DroneLocation& targetLocation)
  : m_targetLocation(targetLocation)
{
}

bool drone::LocationUpdater::updateState(DroneLocation& location,
                                         DroneConfiguration& config)
{
  double x(
    std::abs(m_targetLocation.getXCoordinate() - location.getXCoordinate()));
  double y(
    std::abs(m_targetLocation.getYCoordinate() - location.getYCoordinate()));
  double z(
    std::abs(m_targetLocation.getZCoordinate() - location.getZCoordinate()));
  double total(x + y + z);
  location.setXCoordinate(updateDimension(x / total,
                                          location.getXCoordinate(),
                                          m_targetLocation.getXCoordinate(),
                                          config.getSpeed()));
  location.setYCoordinate(updateDimension(y / total,
                                          location.getYCoordinate(),
                                          m_targetLocation.getYCoordinate(),
                                          config.getSpeed()));
  location.setZCoordinate(updateDimension(z / total,
                                          location.getZCoordinate(),
                                          m_targetLocation.getZCoordinate(),
                                          config.getSpeed()));

  return compareDoubles(
           location.getXCoordinate(), m_targetLocation.getXCoordinate()) &&
         compareDoubles(
           location.getYCoordinate(), m_targetLocation.getYCoordinate()) &&
         compareDoubles(
           location.getZCoordinate(), m_targetLocation.getZCoordinate());
}
