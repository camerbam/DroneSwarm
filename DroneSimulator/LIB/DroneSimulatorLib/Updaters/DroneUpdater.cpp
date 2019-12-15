
#include "DroneUpdater.hpp"

namespace
{
  void fixAngle(double& angle)
  {
    while (angle > 360)
      angle = angle - 360;
    while (angle < 0)
      angle = angle + 360;
  }
}

drone::DroneLocation::DroneLocation()
  : m_xCoordinate(0), m_yCoordinate(0), m_zCoordinate(0), m_angle(0)
{
}

drone::DroneLocation& drone::DroneLocation::operator=(
  const DroneLocation& other)
{
  m_xCoordinate = other.m_xCoordinate;
  m_yCoordinate = other.m_yCoordinate;
  m_zCoordinate = other.m_zCoordinate;
  m_angle = other.m_angle;
  return *this;
}

drone::DroneLocation::DroneLocation(const drone::DroneLocation& location)
  : m_lock(),
    m_xCoordinate(location.m_xCoordinate),
    m_yCoordinate(location.m_yCoordinate),
    m_zCoordinate(location.m_zCoordinate),
    m_angle(location.m_angle)
{
}

double drone::DroneLocation::getXCoordinate()
{
  std::lock_guard<std::mutex> lock(m_lock);
  return m_xCoordinate;
}

double drone::DroneLocation::getYCoordinate()
{
  std::lock_guard<std::mutex> lock(m_lock);
  return m_yCoordinate;
}

double drone::DroneLocation::getZCoordinate()
{
  std::lock_guard<std::mutex> lock(m_lock);
  return m_zCoordinate;
}

double drone::DroneLocation::getAngle()
{
  std::lock_guard<std::mutex> lock(m_lock);
  return m_angle;
}

void drone::DroneLocation::setXCoordinate(double x)
{
  std::lock_guard<std::mutex> lock(m_lock);
  m_xCoordinate = x;
}

void drone::DroneLocation::setYCoordinate(double y)
{
  std::lock_guard<std::mutex> lock(m_lock);
  m_yCoordinate = y;
}

void drone::DroneLocation::setZCoordinate(double z)
{
  std::lock_guard<std::mutex> lock(m_lock);
  if (z < 0) z = 0;
  m_zCoordinate = z;
}

void drone::DroneLocation::setAngle(double angle)
{
  std::lock_guard<std::mutex> lock(m_lock);
  m_angle = angle;
  fixAngle(m_angle);
}
