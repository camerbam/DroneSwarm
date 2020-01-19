
#include "DroneConfiguration.hpp"

#include "RegistryLib/Registry.hpp"

drone::DroneConfiguration::DroneConfiguration(size_t startingBattery)
  : m_speed(10),
    m_battery(startingBattery),
    m_startBattery(startingBattery),
    m_time(0),
  m_detectionDirection(messages::DETECTION_DIRECTION::NONE),
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
  m_time = static_cast<size_t>(
    std::chrono::duration_cast<std::chrono::microseconds>(now - m_startPoint)
      .count() *
    GlobalRegistry::getRegistry().getSpeedRatio() / 1000000);
  m_battery = static_cast<size_t>(
    m_startBattery -
    m_time / GlobalRegistry::getRegistry().getBatteryDecaySpeed());
}

boost::optional<std::string> drone::DroneConfiguration::enableDetection(
  messages::DETECTION_DIRECTION direction)
{
  if (m_detectionDirection == messages::DETECTION_DIRECTION::NONE &&
      direction != messages::DETECTION_DIRECTION::BOTH)
    return std::string(
      "Must enable detection before setting detection direction");
  m_detectionDirection = direction;
  return boost::none;
}

void drone::DroneConfiguration::disableDetection()
{
  m_detectionDirection = messages::DETECTION_DIRECTION::NONE;
}