#include "DroneSimulatorSDKState.hpp"

#include <iostream>
#include <memory>

#include "DroneMessagesLib/MessageFactory.hpp"
#include "DroneSimulatorSDKState.hpp"
#include "DroneSimulatorStateChanges.hpp"
#include "DroneSimulatorStateImpl.hpp"
#include "LoggerLib/Logger.hpp"

drone::DroneSimulatorSDKState::DroneSimulatorSDKState(
  udp::UDPCommunicator& controlEndpoint,
  const boost::asio::ip::udp::endpoint& drone,
  size_t startingBattery,
  int startingY)
  : DroneSimulatorState(controlEndpoint),
    m_pState(
      std::make_shared<DroneSimulatorStateImpl>(startingBattery, startingY)),
    m_statusEndpoint(),
    m_commandCompleted(),
    m_remoteProcessForControl(drone),
    m_remoteProcessForStatus(drone),
    m_statusThread([this, &controlEndpoint]() {
      m_remoteProcessForStatus.port(controlEndpoint.getLocalPort() + 1);
      while (m_pState)
      {
        auto status = m_pState->getStatusMessage();
        m_statusEndpoint.sendMessage(status, m_remoteProcessForStatus);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
      }
    })
{
  m_commandCompleted =
    m_pState->registerForCompletedUpdate([this](std::string message) {
      m_sender.sendMessage(message, m_remoteProcessForControl);
    });
}

drone::DroneSimulatorSDKState::~DroneSimulatorSDKState()
{
  m_pState.reset();
  m_statusThread.join();
}

std::shared_ptr<drone::DroneSimulatorState> drone::DroneSimulatorSDKState::
  handleResponse(const udp::Response& response)
{
  if (!response.didSucceed())
  {
    logger::logError(
      "DroneSimulatorSDKState", "No messages recieved within timeout, exiting");
    return nullptr;
  }
  auto msg = response.getMessage();
  auto optToSend = boost::apply_visitor(
    drone::DroneSimulatorStateChanges(m_pState), messages::getMessage(msg));
  if (optToSend) m_sender.sendMessage(optToSend.get(), response.getEndpoint());
  return shared_from_this();
}
