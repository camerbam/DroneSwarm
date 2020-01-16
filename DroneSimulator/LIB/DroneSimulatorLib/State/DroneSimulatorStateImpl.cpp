#include "DroneSimulatorStateImpl.hpp"

#include <cmath>
#include <condition_variable>
#include <iostream>
#include <map>
#include <mutex>
#include <sstream>

#include <boost/algorithm/string/trim.hpp>

#include "DroneMessagesLib/DroneStatusMessage.hpp"

#include "DroneSimulatorLib/DroneSimulatorConsts.hpp"
#include "DroneSimulatorLib/Updaters/AngleUpdater.hpp"
#include "DroneSimulatorLib/Updaters/LocationUpdater.hpp"

namespace
{
  const std::string BUSY("Drone is already doing something");

  std::string checkDelta(double delta, bool flying)
  {
    if (!flying) return "Drone is not flying";
    if (delta > 500 || delta < -500) return "Distance must be less than 500";
    if (delta > -20 && delta < 20) return "Distance must be greater than 20";
    return "";
  }
} // namespace

drone::DroneSimulatorStateImpl::DroneSimulatorStateImpl(size_t startingBattery)
  : m_flying(false),
    m_statusMutex(),
    m_currentLocation(),
    m_configuration(startingBattery),
    m_pUpdater(),
    m_updateSignal(),
    m_isRunning(true),
    m_updateThread()
{
  startUpdate();
}

drone::DroneSimulatorStateImpl::~DroneSimulatorStateImpl()
{
  std::lock_guard<std::mutex> lock(m_statusMutex);
  m_isRunning = false;
  if (m_updateThread.joinable()) m_updateThread.join();
}

std::string drone::DroneSimulatorStateImpl::takeoff()
{
  if (m_flying) return "Drone is already flying";
  if (m_configuration.getBattery() < 5) return "Drone is out of battery";
  m_flying = true;

  auto targetLocation = m_currentLocation;
  targetLocation.changeZCoordinate(100);
  m_pUpdater = std::make_shared<LocationUpdater>(targetLocation);
  return "";
}

std::string drone::DroneSimulatorStateImpl::land()
{
  if (!m_flying) return "Drone is already landed";
  m_flying = false;
  if (m_pUpdater) return BUSY;
  auto targetLocation = m_currentLocation;
  targetLocation.changeZCoordinate(-100);
  m_pUpdater = std::make_shared<LocationUpdater>(targetLocation);
  return "";
}

std::string drone::DroneSimulatorStateImpl::changeTargetX(double deltaX)
{
  if (!m_flying) return "Drone is not flying";
  auto error = checkDelta(deltaX, m_flying);
  if (!error.empty()) return error;
  if (m_pUpdater) return BUSY;
  auto targetLocation = m_currentLocation;
  targetLocation.changeXCoordinate(deltaX);
  m_pUpdater = std::make_shared<LocationUpdater>(targetLocation);
  return error;
}

double drone::DroneSimulatorStateImpl::getX()
{
  return m_currentLocation.getXCoordinate();
}

std::string drone::DroneSimulatorStateImpl::changeTargetY(double deltaY)
{
  if (!m_flying) return "Drone is not flying";
  auto error = checkDelta(deltaY, m_flying);
  if (!error.empty()) return error;
  if (m_pUpdater) return BUSY;
  auto targetLocation = m_currentLocation;
  targetLocation.changeYCoordinate(deltaY);
  m_pUpdater = std::make_shared<LocationUpdater>(targetLocation);
  return error;
}

double drone::DroneSimulatorStateImpl::getY()
{
  return m_currentLocation.getYCoordinate();
}

std::string drone::DroneSimulatorStateImpl::changeTargetZ(double deltaZ)
{
  if (!m_flying) return "Drone is not flying";
  auto error = checkDelta(deltaZ, m_flying);
  if (!error.empty()) return error;
  if (m_pUpdater) return BUSY;
  auto targetLocation = m_currentLocation;
  targetLocation.changeZCoordinate(deltaZ);
  m_pUpdater = std::make_shared<LocationUpdater>(targetLocation);
  return error;
}

double drone::DroneSimulatorStateImpl::getZ()
{
  return m_currentLocation.getZCoordinate();
}

std::string drone::DroneSimulatorStateImpl::changeTargetXYZ(double deltaX,
                                                            double deltaY,
                                                            double deltaZ,
                                                            size_t deltaSpeed)
{
  if (!m_flying) return "Drone is not flying";
  auto error = checkDelta(deltaX, m_flying);
  if (!error.empty()) return error;
  error = checkDelta(deltaY, m_flying);
  if (!error.empty()) return error;
  error = checkDelta(deltaZ, m_flying);
  if (!error.empty()) return error;
  if (m_pUpdater) return BUSY;
  auto targetLocation = m_currentLocation;
  targetLocation.changeXCoordinate(deltaX);
  targetLocation.changeYCoordinate(deltaY);
  targetLocation.changeZCoordinate(deltaZ);
  changeSpeed(deltaSpeed);
  m_pUpdater = std::make_shared<LocationUpdater>(targetLocation);
  return "";
}

std::string drone::DroneSimulatorStateImpl::changeTargetAngle(double deltaAngle)
{
  if (!m_flying) return "Drone is not flying";
  if (deltaAngle < 1 && deltaAngle > -1)
    return "Angle must be greater than 1 degree";
  if (deltaAngle > 360 || deltaAngle < -360)
    return "Angle must be less than 360 degree";

  if (m_pUpdater) return BUSY;
  m_pUpdater = std::make_shared<AngleUpdater>(deltaAngle);

  return "";
}

double drone::DroneSimulatorStateImpl::getTimeOfFlight()
{
  return m_currentLocation.getTimeOfFlight();
}

double drone::DroneSimulatorStateImpl::getAngle()
{
  return m_currentLocation.getAngle();
}

std::string drone::DroneSimulatorStateImpl::changeSpeed(size_t newSpeed)
{
  if (newSpeed > 100) return "Speed must be less than 100";
  if (newSpeed < 10) return "Speed must be greater than 10";
  m_configuration.setSpeed(newSpeed);
  return "";
}

size_t drone::DroneSimulatorStateImpl::getSpeed()
{
  return m_configuration.getSpeed();
}

size_t drone::DroneSimulatorStateImpl::getBattery() const
{
  return m_configuration.getBattery();
}

size_t drone::DroneSimulatorStateImpl::getTime()
{
  return m_configuration.getTime();
}

std::string drone::DroneSimulatorStateImpl::getStatusMessage()
{
  messages::DroneStatusMessage msg;
  std::lock_guard<std::mutex> lock(m_statusMutex);
  return msg.toString(m_currentLocation.getMid(), getX(),
                      getY(),
                      getZ(),
                      getTimeOfFlight(),
                      getBattery(),
                      getTime());
}

boost::optional<std::string> drone::DroneSimulatorStateImpl::enableDetection(
  messages::DETECTION_DIRECTION direction)
{
  return m_configuration.enableDetection(direction);
}

void drone::DroneSimulatorStateImpl::disableDetection()
{
  m_configuration.enableDetection(messages::DETECTION_DIRECTION::NONE);
}

boost::signals2::scoped_connection drone::DroneSimulatorStateImpl::
  registerForCompletedUpdate(std::function<void(std::string)> callback)
{
  return m_updateSignal.connect(callback);
}

void drone::DroneSimulatorStateImpl::startUpdate()
{
  m_updateThread = std::thread([this]() {
    while (m_isRunning)
    {
      auto now = std::chrono::steady_clock::now();
      m_configuration.update(now);
      if (m_pUpdater &&
          m_pUpdater->updateState(m_currentLocation, m_configuration))
      {
        m_pUpdater.reset();
        m_updateSignal("ok");
      }

      std::this_thread::sleep_for(
        std::chrono::microseconds(1000000) / drone::FRAMES_PER_SECOND -
        std::chrono::duration_cast<std::chrono::microseconds>(
          std::chrono::steady_clock::now() - now));
    }
  });
}
