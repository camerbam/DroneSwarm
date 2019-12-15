#include "DroneController.hpp"

#include <iostream>

#include "DroneControllerMessagesToString.hpp"
#include "DroneControllerStateChanges.hpp"
#include "DroneMessagesLib/Messages/BatteryMessage.hpp"
#include "DroneMessagesLib/Messages/CommandMessage.hpp"
#include "UDPLib/Response.hpp"

drone::DroneController::DroneController(const std::string& ipAddress)
  : m_pState(std::make_shared<drone::DroneControllerState>()),
    m_running(true),
    m_controlCommunicator(),
    m_controlEndpoint(boost::asio::ip::address::from_string(ipAddress), 8889),
    m_statusCommunicator(8890),
    m_connection(),
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
      }
    })
{
  m_controlCommunicator.startPing(
    [this]() {
      auto battery = std::make_shared<messages::BatteryMessage>();
      auto response = m_controlCommunicator.sendMessage(
        battery->toString(), m_controlEndpoint);
    },
    boost::posix_time::seconds(8));
  messages::Message_t command = messages::CommandMessage();
  sendMessage(command);

  m_connection = m_pState->registerForBattery([this](size_t battery) {
    if (battery >= 20) return;
    m_running = false;
  });
}

drone::DroneController::~DroneController()
{
  m_running = false;
  m_statusThread.join();
}

boost::optional<std::string> drone::DroneController::sendMessage(
  const messages::Message_t& message)
{
  if (!m_running) return boost::none;
  auto str =
    boost::apply_visitor(DroneControllerMessagesToString(m_pState), message);
  auto response = m_controlCommunicator.sendMessage(str, m_controlEndpoint);
  if (!response.didSucceed())
  {
    return boost::none;
  }
  boost::apply_visitor(DroneControllerStateChanges(m_pState), message);
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
