#ifndef DRONE_SIMULATOR_PRETEST_STATE_HPP
#define DRONE_SIMULATOR_PRETEST_STATE_HPP

#include "DroneSimulatorState.hpp"
#include "DroneSimulatorStateImpl.hpp"

#include "UDPLib/Response.hpp"
#include "UDPLib/UDPCommunicator.hpp"

namespace drone
{
  class DroneSimulatorPretestState : public DroneSimulatorState
  {
  public:
    DroneSimulatorPretestState(udp::UDPCommunicator& controlEndpoint,
      const boost::asio::ip::udp::endpoint& droneEndpoint, bool reliable);

    ~DroneSimulatorPretestState();

    virtual std::shared_ptr<drone::DroneSimulatorState> handleResponse(
      const udp::Response& response) override;

  private:
    const boost::asio::ip::udp::endpoint m_droneEndpoint;
    int m_reliable;
  };
} // namespace drone

#endif
