
#include "DroneConfiguration.hpp"


drone::DroneConfiguration::DroneConfiguration(size_t startingBattery)
  : m_speed(10),
    m_battery(startingBattery),
    m_startBattery(startingBattery),
    m_time(0),
    m_startPoint(std::chrono::steady_clock::now())
{
}

drone::DroneConfiguration::DroneConfiguration(
  const drone::DroneConfiguration& location)
  : m_speed(location.getSpeed()),
    m_battery(location.getBattery()),
    m_startBattery(location.getBattery()),
    m_time(location.getTime())
{
}

size_t drone::DroneConfiguration::getSpeed() const
{
  return m_speed;
}

size_t drone::DroneConfiguration::getBattery() const
{
  return m_battery;
}

size_t drone::DroneConfiguration::getTime() const
{
  return m_time;
}

std::chrono::steady_clock::time_point drone::DroneConfiguration::getStartPoint()
  const
{
  return m_startPoint;
}

void drone::DroneConfiguration::setSpeed(size_t speed)
{
  m_speed = speed;
}

void drone::DroneConfiguration::decreaseBattery()
{
  m_battery--;
}

void drone::DroneConfiguration::increaseTime()
{
  m_time++;
}

void drone::DroneConfiguration::update(
  const std::chrono::steady_clock::time_point& now)
{
  m_time = std::chrono::duration_cast<std::chrono::seconds>(now - m_startPoint)
             .count();
  m_battery = m_startBattery - m_time / 2;
}
