#include "DroneControllerState.hpp"

#include <algorithm>
#include <cmath>
#include <sstream>

#include <boost/algorithm/string/trim.hpp>

#include "DroneMessagesLib/DroneStatusMessage.hpp"

namespace
{
  void fixAngle(std::atomic<double>& angle)
  {
    while (angle > 360)
      angle = angle - 360;
    while (angle < 0)
      angle = angle + 360;
  }
} // namespace

drone::DroneControllerState::DroneControllerState(size_t startBattery)
  : m_flying(false),
    m_statusMutex(),
    m_mid(-1),
    m_direction(messages::DETECTION_DIRECTION::NONE),
    m_xCoordinate(0),
    m_yCoordinate(0),
    m_zCoordinate(0),
    m_timeOfFlight(0),
    m_angle(0),
    m_speed(10),
    m_battery(startBattery),
    m_time(0),
    m_batterySignal(),
    m_midSignal(),
    m_knownTargets()
{
}

drone::DroneControllerState::~DroneControllerState()
{
}

void drone::DroneControllerState::takeoff()
{
  m_flying = true;
}

void drone::DroneControllerState::land()
{
  m_flying = false;
}

int drone::DroneControllerState::getMid()
{
  return m_mid;
}

double drone::DroneControllerState::getX()
{
  return m_xCoordinate;
}

double drone::DroneControllerState::getY()
{
  return m_yCoordinate;
}

double drone::DroneControllerState::getZ()
{
  return m_zCoordinate;
}

double drone::DroneControllerState::getTimeOfFlight()
{
  return m_timeOfFlight;
}

double drone::DroneControllerState::getAngle()
{
  return m_angle;
}

void drone::DroneControllerState::changeSpeed(size_t newSpeed)
{
  m_speed = newSpeed;
}

void drone::DroneControllerState::setDetection(
  const messages::DETECTION_DIRECTION& direction)
{
  m_direction = direction;
}

size_t drone::DroneControllerState::getSpeed()
{
  return m_speed;
}

size_t drone::DroneControllerState::getBattery() const
{
  return m_battery;
}

size_t drone::DroneControllerState::getTime()
{
  return m_time;
}

bool drone::DroneControllerState::isFlying()
{
  return m_flying;
}

messages::DETECTION_DIRECTION drone::DroneControllerState::getDirection()
{
  return m_direction;
}

bool drone::DroneControllerState::updateStatus(const std::string& statusMessage)
{
  if (statusMessage.empty()) return false;
  messages::DroneStatusMessage msg;
  msg.fromString(statusMessage);
  auto newBattery = msg.getBattery();
  if (newBattery != m_battery)
  {
    m_battery = newBattery;
    m_batterySignal(m_battery);
  }
  m_timeOfFlight = msg.getTimeOfFlight();
  auto mid = msg.getMid();
  if (m_mid != mid)
  {
    m_midSignal(mid);
    m_knownTargets.emplace_back(
      m_xCoordinate, m_yCoordinate, mid);
  }
  m_mid = mid;

  auto currentTarget = std::find_if(
    m_knownTargets.begin(), m_knownTargets.end(), [mid](const Target& target) {
      return mid == target.getId();
    });
  m_xCoordinate = msg.getXCoordinate() + currentTarget->getX();
  m_yCoordinate = msg.getYCoordinate() + currentTarget->getY();
  static int i = 0;
  i++;
  m_zCoordinate = msg.getZCoordinate();
  m_angle = msg.getAngle();
  m_time = msg.getTime();

  return true;
}

boost::signals2::scoped_connection drone::DroneControllerState::
  registerForBattery(std::function<void(size_t)> callback)
{
  return m_batterySignal.connect(callback);
}

boost::signals2::scoped_connection drone::DroneControllerState::registerForMid(
  std::function<void(size_t)> callback)
{
  return m_midSignal.connect(callback);
}
