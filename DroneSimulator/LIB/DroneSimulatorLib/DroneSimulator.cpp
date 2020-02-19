#include "DroneSimulator.hpp"

#include <functional>

#include "DroneMessagesLib/MessageFactory.hpp"
#include "UDPLib/Response.hpp"

#include "LoggerLib/Logger.hpp"
#include "RegistryLib/Registry.hpp"
#include "State/DroneSimulatorIdleState.hpp"
#include "State/DroneSimulatorStateChanges.hpp"

#include <iostream>
#include <string>

drone::DroneSimulator::DroneSimulator(unsigned short port,
                                      boost::posix_time::seconds timeout,
                                      size_t startingBattery,
  int startingY)
  : m_controlEndpoint(port),
    m_pState(std::make_shared<DroneSimulatorIdleState>(
      m_controlEndpoint, startingBattery, startingY)),
    m_controlThread([this, timeout]() {
      while (m_pState)
      {
        udp::Response response(m_controlEndpoint.receiveMessage(timeout));
        logger::logInfo("DroneSimulator", "Received: " + response.getMessage());
        m_pState = m_pState->handleResponse(response);
      }
    })
{
}

drone::DroneSimulator::~DroneSimulator()
{
  m_controlThread.join();
}