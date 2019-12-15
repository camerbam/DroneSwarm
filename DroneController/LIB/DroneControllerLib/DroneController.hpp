#ifndef DRONE_CONTROLLER_HPP
#define DRONE_CONTROLLER_HPP

#include <boost/optional.hpp>
#include <boost/signals2/connection.hpp>

#include "DroneControllerState.hpp"
#include "DroneMessagesLib/Message_t.hpp"
#include "DroneMessagesLib/Message.hpp"
#include "UDPLib/UDPCommunicator.hpp"
#include "UDPLib/UDPCommunicatorReliable.hpp"

namespace drone
{
  class DroneController
  {
  public:
    DroneController(const std::string& ipAddress = std::string("127.0.0.1"));

    ~DroneController();

    boost::optional<std::string> sendMessage(
      const messages::Message_t& message);

    size_t getBattery();
    double getX();
    double getY();
    double getZ();
    size_t getSpeed();
    size_t getTime();
    double getTimeOfFlight();
    bool getIsRunning();

  private:
    std::shared_ptr<DroneControllerState> m_pState;
    std::atomic<bool> m_running;
    udp::UDPCommunicatorReliable m_controlCommunicator;
    boost::asio::ip::udp::endpoint m_controlEndpoint;
    udp::UDPCommunicator m_statusCommunicator;
    boost::signals2::scoped_connection m_connection;
    std::thread m_statusThread;
  };
} // namespace drone

#endif
