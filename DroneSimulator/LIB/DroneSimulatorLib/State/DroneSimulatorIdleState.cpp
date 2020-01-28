#include "DroneSimulatorIdleState.hpp"

#include <iostream>

#include "DroneMessagesLib/MessageFactory.hpp"
#include "DroneSimulatorPretestState.hpp"
#include "DroneSimulatorSDKState.hpp"
#include "RegistryLib/Registry.hpp"

namespace
{
  class DroneSimulatorIdleStateChanges
    : public boost::static_visitor<std::shared_ptr<drone::DroneSimulatorState>>
  {
  public:
    DroneSimulatorIdleStateChanges(udp::UDPCommunicator& controlEndpoint,
                                   const udp::Response& response,
                                   size_t startingBattery)
      : m_controlEndpoint(controlEndpoint),
        m_response(response),
        m_startingBattery(startingBattery)
    {
    }

    std::shared_ptr<drone::DroneSimulatorState> operator()(
      const messages::CommandMessage&) const
    {
      std::shared_ptr<drone::DroneSimulatorState> toReturn;
      switch (GlobalRegistry::getRegistry().getPretest())
      {
      case 0:
        toReturn = std::make_shared<drone::DroneSimulatorSDKState>(
          m_controlEndpoint, m_response.getEndpoint(), m_startingBattery);
        break;
      case 1:
        toReturn = std::make_shared<drone::DroneSimulatorPretestState>(
          m_controlEndpoint, m_response.getEndpoint());
        break;
      case 2:
        break;
      default:
        m_controlEndpoint.sendMessage(
          "No known pretest: " +
            std::to_string(GlobalRegistry::getRegistry().getPretest()),
          m_response.getEndpoint());
        return nullptr;
      }
      m_controlEndpoint.sendMessage("ok", m_response.getEndpoint());
      return toReturn;
    }

    template <class T>
    std::shared_ptr<drone::DroneSimulatorState> operator()(const T&) const
    {
      std::cout << "Cannot receive messages other than command in Idle mode"
                << std::endl;
      return nullptr;
    }

  private:
    udp::UDPCommunicator& m_controlEndpoint;
    const udp::Response m_response;
    size_t m_startingBattery;
  };
}

drone::DroneSimulatorIdleState::DroneSimulatorIdleState(
  udp::UDPCommunicator& controlEndpoint, size_t startingBattery)
  : DroneSimulatorState(controlEndpoint), m_startingBattery(startingBattery)
{
}

drone::DroneSimulatorIdleState::~DroneSimulatorIdleState()
{
}

std::shared_ptr<drone::DroneSimulatorState> drone::DroneSimulatorIdleState::
  handleResponse(const udp::Response& response)
{
  if (!response.didSucceed()) return shared_from_this();
  auto msg = response.getMessage();
  auto newState = boost::apply_visitor(
    DroneSimulatorIdleStateChanges(m_sender, response, m_startingBattery),
    messages::getMessage(msg));
  if (!newState) return shared_from_this();
  return newState;
}
