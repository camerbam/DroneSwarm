#include "DroneControllerState.hpp"

#include <cmath>
#include <sstream>

#include <boost/algorithm/string/trim.hpp>

#include "DroneMessagesLib/DroneStatusMessage.hpp"

namespace
{
  std::string changeCoordinate(double delta, bool flying)
  {
    if (!flying) return "Drone is not flying";
    if (delta > 500 || delta < -500) return "Distance must be less than 500";
    if (delta > -20 && delta < 20) return "Distance must be greater than 20";
    return "";
  }

  void fixAngle(std::atomic<double>& angle)
  {
    while (angle > 360)
      angle = angle - 360;
    while (angle < 0)
      angle = angle + 360;
  }
} // namespace

drone::DroneControllerState::DroneControllerState()
  : m_flying(false),
    m_statusMutex(),
    m_mid(-1),
    m_xCoordinate(0),
    m_yCoordinate(0),
    m_zCoordinate(0),
    m_timeOfFlight(0),
    m_angle(0),
    m_speed(10),
    m_battery(100),
    m_time(0)
{
}

drone::DroneControllerState::~DroneControllerState()
{
}

std::string drone::DroneControllerState::takeoff()
{
  if (m_flying) return "Drone is already flying";
  if (m_battery < 5) return "Drone is out of battery";
  m_flying = true;
  m_zCoordinate = 100;
  return "";
}

std::string drone::DroneControllerState::land()
{
  if (!m_flying) return "Drone is already landed";
  m_flying = false;
  m_zCoordinate = 0;
  return "";
}

int drone::DroneControllerState::getMid()
{
  return m_mid;
}

std::string drone::DroneControllerState::changeX(double deltaX)
{
  auto error = changeCoordinate(deltaX, m_flying);
  if (error.empty())
  {
    m_xCoordinate = m_xCoordinate + deltaX;
  }
  return error;
}

double drone::DroneControllerState::getX()
{
  return m_xCoordinate;
}

std::string drone::DroneControllerState::changeY(double deltaY)
{
  auto error = changeCoordinate(deltaY, m_flying);
  if (error.empty())
  {
    m_yCoordinate = m_yCoordinate + deltaY;
  }
  return error;
}

double drone::DroneControllerState::getY()
{
  return m_yCoordinate;
}

std::string drone::DroneControllerState::changeZ(double deltaZ)
{
  auto error = changeCoordinate(deltaZ, m_flying);
  if (error.empty())
  {
    if (m_zCoordinate + deltaZ < 0) deltaZ = -m_zCoordinate;
    m_zCoordinate = m_zCoordinate + deltaZ;
  }
  return error;
}

double drone::DroneControllerState::getZ()
{
  return m_zCoordinate;
}

std::string drone::DroneControllerState::changeXYZ(double deltaX,
                                                   double deltaY,
                                                   double deltaZ)
{
  if (!m_flying) return "Drone is not flying";
  if (m_zCoordinate + deltaZ < 0) deltaZ = -m_zCoordinate;
  auto error = changeCoordinate(deltaZ, m_flying);
  if (!error.empty()) return error;
  error = changeCoordinate(deltaY, m_flying);
  if (!error.empty()) return error;
  error = changeCoordinate(deltaZ, m_flying);
  if (!error.empty()) return error;
  m_xCoordinate = m_xCoordinate + deltaX;
  m_yCoordinate = m_yCoordinate + deltaY;
  m_zCoordinate = m_zCoordinate + deltaZ;

  return "";
}

std::string drone::DroneControllerState::changeAngle(double deltaAngle)
{
  if (!m_flying) return "Drone is not flying";
  if (deltaAngle < 1 && deltaAngle > -1)
    return "Angle must be greater than 1 degree";
  if (deltaAngle > 360 || deltaAngle < -360)
    return "Angle must be less than 360 degree";
  m_angle = m_angle + deltaAngle;
  fixAngle(m_angle);
  return "";
}

double drone::DroneControllerState::getTimeOfFlight()
{
  return m_timeOfFlight;
}

double drone::DroneControllerState::getAngle()
{
  return m_angle;
}

std::string drone::DroneControllerState::changeSpeed(size_t newSpeed)
{
  if (newSpeed > 100) return "Speed must be less than 100";
  if (newSpeed < 10) return "Speed must be greater than 10";
  m_speed = newSpeed;
  return "";
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

bool drone::DroneControllerState::updateStatus(std::string& statusMessage)
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
  m_mid = msg.getMid();
  m_xCoordinate = msg.getXCoordinate();
  m_yCoordinate = msg.getYCoordinate();
  m_zCoordinate = msg.getZCoordinate();
  m_time = msg.getTime();

  return true;
}

boost::signals2::scoped_connection drone::DroneControllerState::
  registerForBattery(std::function<void(size_t)> callback)
{
  return m_batterySignal.connect(callback);
}
