
#include "DroneLocation.hpp"

#include <mutex>

namespace
{
  void fixAngle(std::atomic<double>& angle)
  {
    while (angle > 360)
      angle = angle - 360;
    while (angle < 0)
      angle = angle + 360;
  }

  double calculateDiff(const double& a, const double& b)
  {
    return std::abs(a - b);
  }
}

drone::DroneLocation::DroneLocation()
  : m_timeOfFlight(0),
    m_mid(0),
    m_xCoordinate(0),
    m_yCoordinate(0),
    m_zCoordinate(0),
    m_angle(0)
{
}

drone::DroneLocation::DroneLocation(const drone::DroneLocation& location)
  : m_timeOfFlight(location.getTimeOfFlight()),
    m_xCoordinate(location.getXCoordinate()),
    m_yCoordinate(location.getYCoordinate()),
    m_zCoordinate(location.getZCoordinate()),
    m_angle(location.getAngle())
{
}

double drone::DroneLocation::getTimeOfFlight() const
{
  return m_timeOfFlight;
}

double drone::DroneLocation::getMid() const
{
  return m_mid;
}

double drone::DroneLocation::getXCoordinate() const
{
  return m_xCoordinate;
}

double drone::DroneLocation::getYCoordinate() const
{
  return m_yCoordinate;
}

double drone::DroneLocation::getZCoordinate() const
{
  return m_zCoordinate;
}

double drone::DroneLocation::getAngle() const
{
  return m_angle;
}

void drone::DroneLocation::setMid(double x)
{
  m_mid = x;
}

void drone::DroneLocation::setXCoordinate(double x)
{
  addTimeOfFlight(calculateDiff(x, m_xCoordinate));
  m_xCoordinate = x;
}

void drone::DroneLocation::setYCoordinate(double y)
{
  addTimeOfFlight(calculateDiff(y, m_yCoordinate));
  m_yCoordinate = y;
}

void drone::DroneLocation::setZCoordinate(double z)
{
  if (z < 0) z = 0;
  addTimeOfFlight(calculateDiff(z, m_zCoordinate));
  m_zCoordinate = z;
}

void drone::DroneLocation::setXYZCoordinate(double x, double y, double z)
{
  if (z < 0) z = 0;
  auto diffX = calculateDiff(x, m_xCoordinate);
  auto diffY = calculateDiff(y, m_yCoordinate);
  auto diffZ = calculateDiff(z, m_zCoordinate);
  addTimeOfFlight(
    std::sqrt(std::pow(diffX, 2) + std::pow(diffY, 2) + std::pow(diffZ, 2)));
  m_xCoordinate = x;
  m_yCoordinate = y;
  m_zCoordinate = z;
}

void drone::DroneLocation::setAngle(double angle)
{
  m_angle = angle;
  fixAngle(m_angle);
}

void drone::DroneLocation::addTimeOfFlight(double timeToAdd)
{
  if (timeToAdd < 0) timeToAdd *= -1;
  m_timeOfFlight = m_timeOfFlight + timeToAdd;
}

void drone::DroneLocation::changeXCoordinate(double x)
{
  addTimeOfFlight(x);
  m_xCoordinate = m_xCoordinate + x;
}

void drone::DroneLocation::changeYCoordinate(double y)
{
  addTimeOfFlight(y);
  m_yCoordinate = m_yCoordinate + y;
}

void drone::DroneLocation::changeZCoordinate(double z)
{
  addTimeOfFlight(std::abs(z));
  m_zCoordinate = m_zCoordinate + z;
  if (m_zCoordinate < 0) m_zCoordinate = 0;
}

void drone::DroneLocation::changeXYZCoordinate(double x, double y, double z)
{
  addTimeOfFlight(std::sqrt(std::pow(x, 2) + std::pow(y, 2) + std::pow(z, 2)));

  m_xCoordinate = m_xCoordinate + x;
  m_yCoordinate = m_yCoordinate + y;
  m_zCoordinate = m_zCoordinate + z;
  if (m_zCoordinate < 0) m_zCoordinate = 0;
}

void drone::DroneLocation::changeAngle(double angle)
{
  m_angle = m_angle + angle;
  fixAngle(m_angle);
}
