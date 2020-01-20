
#include "AngleUpdater.hpp"

#include "DroneSimulatorLib/DroneSimulatorConsts.hpp"

namespace
{
  // The is no config for angle speed
  const double DEG_PRE_SEC(90);
  const double DEG_PER_FRAME(DEG_PRE_SEC / drone::FRAMES_PER_SECOND);
  const double HALF_CIRCLE(180);
  const double FULL_CIRCLE(360);
}

drone::AngleUpdater::AngleUpdater(const double& targetAngle)
  : m_targetAngle(targetAngle)
{
}

bool drone::AngleUpdater::updateState(DroneLocation& location,
                                      DroneConfiguration&)
{
  auto angle = location.getAngle();
  double angleDiff(m_targetAngle - angle);
  if (angleDiff > HALF_CIRCLE) angle -= FULL_CIRCLE;
  if (angleDiff < -HALF_CIRCLE) angle += FULL_CIRCLE;

  if (angle + DEG_PER_FRAME < m_targetAngle)
  {
    location.changeAngle(DEG_PER_FRAME);
    return false;
  }
  else
  {
    location.setAngle(m_targetAngle);
    return true;
  }
}
