#ifndef DRONE_SIMULATOR_STATE_HPP
#define DRONE_SIMULATOR_STATE_HPP

#include <memory>

#include <boost/optional.hpp>

#include "UDPLib/Response.hpp"
#include "UDPLib/UDPCommunicator.hpp"

namespace drone
{
  class DroneState;

  class DroneSimulatorState
    : public std::enable_shared_from_this<DroneSimulatorState>
  {
  public:
    DroneSimulatorState(udp::UDPCommunicator& controlEndpoint)
      : m_sender(controlEndpoint)
    {
    }

    ~DroneSimulatorState() {}

    virtual std::shared_ptr<drone::DroneSimulatorState> handleResponse(
      const udp::Response& response) = 0;

  protected:
    udp::UDPCommunicator& m_sender;
  };
} // namespace drone

#endif
