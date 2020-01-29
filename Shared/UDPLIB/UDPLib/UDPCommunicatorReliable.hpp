#ifndef UDP_ENDPOINT_RELIABLE_HPP
#define UDP_ENDPOINT_RELIABLE_HPP

#include "UDPCommunicator.hpp"

#include <chrono>

namespace udp
{
  class UDPCommunicatorReliable
  {
  public:
    /**
     * Constructor for UDPCommunicatorReliable
     * @param[in] remoteEndpoint - where to send packets
     */
    UDPCommunicatorReliable();

    ~UDPCommunicatorReliable();

    UDPCommunicatorReliable(const UDPCommunicatorReliable&) = delete;

    Response sendMessage(const std::string& msgToSend,
                         const boost::asio::ip::udp::endpoint& remoteEndpoint,
                         const boost::posix_time::time_duration& timeout =
                           boost::posix_time::seconds(10),
                         std::function<bool(const std::string& msg)> validator =
                           [](...) { return true; },
                         size_t tries = 1);

    void startPing(std::function<void()> pingFunction,
                   const boost::posix_time::time_duration& pingTime);

  private:
    UDPCommunicator m_sender;
    boost::posix_time::ptime m_lastSent;
    std::mutex m_sending;
    std::atomic<bool> m_running;
    std::shared_ptr<std::thread> m_pPingThread;
  };
}

#endif
