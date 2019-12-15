#ifndef DRONE_SIMULATOR_IDLE_STATE_HPP
#define DRONE_SIMULATOR_IDLE_STATE_HPP

#include "UDPLib/Response.hpp"

#include "DroneSimulatorState.hpp"

namespace drone
{
  class DroneSimulatorIdleState : public DroneSimulatorState
  {
  public:
    DroneSimulatorIdleState(udp::UDPCommunicator& controlEndpoint,
                            size_t startingBattery = 100);

    ~DroneSimulatorIdleState();

    virtual std::shared_ptr<drone::DroneSimulatorState> handleResponse(
      const udp::Response& response) override;

  private:
    size_t m_startingBattery;
  };
} // namespace drone

#endif