#include "DroneController.hpp"

#include <iostream>

#include "DroneControllerCheckMsgToSend.hpp"
#include "DroneControllerMessagesToString.hpp"
#include "DroneControllerStateChanges.hpp"
#include "DroneMessagesLib/Messages/BatteryMessage.hpp"
#include "DroneMessagesLib/Messages/CommandMessage.hpp"
#include "LoggerLib/Logger.hpp"
#include "UDPLib/Response.hpp"

namespace
{
  const std::string DRONE_CONTROLLER("Drone Controller");
}

drone::DroneController::DroneController(logger::MonitorLogger& logger,
                                        unsigned short dronePort,
                                        unsigned short droneStatusPort,
                                        const std::string& ipAddress,
                                        int startingY)
  : m_logger(logger),
    m_pState(std::make_shared<drone::DroneControllerState>(100, startingY)),
    m_running(true),
    m_controlCommunicator(),
    m_controlEndpoint(
      boost::asio::ip::address::from_string(ipAddress), dronePort),
    m_statusCommunicator(droneStatusPort),
    m_connection(),
    m_cvStatus(),
    m_midSignal(),
    m_midConnection(),
    m_mids(),
    m_statusMutex(),
    m_statusThread([this]() {
      while (m_running)
      {
        auto status =
          m_statusCommunicator.receiveMessage(boost::posix_time::seconds(1));
        if (!status.didSucceed() && m_running)
        {
          continue;
        }
        else if (!status.didSucceed())
          return;
        std::string statusMsg = status.getMessage();
        m_pState->updateStatus(statusMsg);
        m_cvStatus.notify_one();
      }
    })
{
  m_controlCommunicator.startPing(
    [this]() {
      auto battery = std::make_shared<messages::BatteryMessage>();
      m_logger.logInfo(DRONE_CONTROLLER, battery->toString());
      auto response = m_controlCommunicator.sendMessage(
        battery->toString(), m_controlEndpoint);
      if (response.didSucceed())
        m_logger.logInfo(DRONE_CONTROLLER, response.getMessage());
    },
    boost::posix_time::seconds(8));

  m_midConnection =
    m_pState->registerForMid([this](int id) { m_mids.push_back(id); });

  messages::Message_t command = messages::CommandMessage();
  sendMessage(command);
}

drone::DroneController::~DroneController()
{
  m_running = false;
  m_statusThread.join();
}

boost::optional<std::string> drone::DroneController::sendMessage(
  const messages::Message_t& message, boost::posix_time::time_duration timeout)
{
  if (!m_running) return boost::none;
  auto error =
    boost::apply_visitor(DroneControllerCheckMsgToSend(m_pState), message);
  if (error)
  {
    m_logger.logError(DRONE_CONTROLLER, error.get());
    return boost::none;
  }
  auto str =
    boost::apply_visitor(DroneControllerMessagesToString(m_pState), message);
  m_logger.logInfo(DRONE_CONTROLLER, str);
  auto response =
    m_controlCommunicator.sendMessage(str, m_controlEndpoint, timeout);
  if (!response.didSucceed())
  {
    m_logger.logError(DRONE_CONTROLLER, response.getMessage());
    return boost::none;
  }
  else
  {
    m_logger.logInfo(DRONE_CONTROLLER, response.getMessage());
  }
  boost::apply_visitor(DroneControllerStateChanges(m_pState), message);

  if (!m_mids.empty())
  {
    for (auto&& id : m_mids)
      m_midSignal(id);
    m_mids.clear();
  }

  return response.getMessage();
}

size_t drone::DroneController::getBattery()
{
  return m_pState->getBattery();
}

double drone::DroneController::getX()
{
  return m_pState->getX();
}

double drone::DroneController::getY()
{
  return m_pState->getY();
}

double drone::DroneController::getZ()
{
  return m_pState->getZ();
}

size_t drone::DroneController::getSpeed()
{
  return m_pState->getSpeed();
}

size_t drone::DroneController::getTime()
{
  return m_pState->getTime();
}

double drone::DroneController::getTimeOfFlight()
{
  return m_pState->getTimeOfFlight();
}

bool drone::DroneController::getIsRunning()
{
  return m_running;
}

void drone::DroneController::waitForStatusMsg()
{
  std::unique_lock<std::mutex> lock(m_statusMutex);
  m_cvStatus.wait_for(lock, std::chrono::seconds(1));
}

bool drone::DroneController::isFlying()
{
  return m_pState->isFlying();
}

void drone::DroneController::stopRunning()
{
  m_running = false;
}

boost::signals2::scoped_connection drone::DroneController::registerForMid(
  std::function<void(int)> callback)
{
  return m_midSignal.connect(callback);
}
