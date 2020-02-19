#ifndef DRONE_SIMULATOR_HPP
#define DRONE_SIMULATOR_HPP


#include "State/DroneSimulatorState.hpp"
#include "UDPLib/UDPCommunicator.hpp"

namespace drone
{
  class DroneSimulator
  {
  public:
    DroneSimulator(
      unsigned short port,
      boost::posix_time::seconds timeout = boost::posix_time::seconds(15),
      size_t startingBattery = 100,
      int startingY = 0);

    ~DroneSimulator();

  private:
    udp::UDPCommunicator m_controlEndpoint;
    std::shared_ptr<DroneSimulatorState> m_pState;
    std::thread m_controlThread;
  };
} // namespace drone

#endif
