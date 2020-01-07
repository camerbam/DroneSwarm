#ifndef DRONE_SIMULATOR_SDK_STATE_HPP
#define DRONE_SIMULATOR_SDK_STATE_HPP

#include "DroneSimulatorState.hpp"
#include "DroneSimulatorStateImpl.hpp"

#include "UDPLib/Response.hpp"
#include "UDPLib/UDPCommunicator.hpp"

namespace drone
{
  class DroneSimulatorSDKState : public DroneSimulatorState
  {
  public:
    DroneSimulatorSDKState(udp::UDPCommunicator& controlEndpoint,
                           const boost::asio::ip::udp::endpoint& drone,
                           size_t startingBattery);

    ~DroneSimulatorSDKState();

    virtual std::shared_ptr<drone::DroneSimulatorState> handleResponse(
      const udp::Response& response) override;

  private:
    std::shared_ptr<drone::DroneSimulatorStateImpl> m_pState;
    udp::UDPCommunicator m_statusEndpoint;
    boost::signals2::scoped_connection m_commandCompleted;
    boost::asio::ip::udp::endpoint m_remoteProcessForControl;
    boost::asio::ip::udp::endpoint m_remoteProcessForStatus;
    std::thread m_statusThread;
  };
} // namespace drone

#endif
