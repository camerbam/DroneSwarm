#ifndef UDP_ENDPOINT_HPP
#define UDP_ENDPOINT_HPP

#include <thread>

#include <boost/asio.hpp>
#include <boost/optional.hpp>

namespace udp
{
  class Response;

  class UDPCommunicator
  {
  public:
    /**
    * Constructor for UDPSendReceive
    * Binds to any open local port
    */
    UDPCommunicator();
    /**
     * Constructor for UDPSendReceive
     * @param[in] localPort - port where to receive packets
     */
    UDPCommunicator(const short localPort);

    UDPCommunicator(const UDPCommunicator&) = delete;

    ~UDPCommunicator();

    /**
     * Sends a string to the remote endpoint
     * @param[in] toSend - data to send to remote endpoint
     */
    size_t sendMessage(const std::string& toSend,
                       const boost::asio::ip::udp::endpoint& remoteEndpoint);

    /**
     *  Receives a message from the reomte endpoint
     * @return - message received and if it failed
     */
    Response receiveMessage(const boost::posix_time::time_duration& timeout =
                              boost::posix_time::seconds(10));

  private:
    boost::asio::io_context m_ctx;
    boost::optional<boost::asio::io_context::work> m_optCork;
    boost::asio::ip::udp::socket m_socket;
    std::thread m_ctxThread;
  };
}

#endif
