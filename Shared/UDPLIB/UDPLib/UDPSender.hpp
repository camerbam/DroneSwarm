#include <string>

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/optional.hpp>
#include <boost/signals2.hpp>

#include <ACQLib/ACQEndpoint.hpp>

namespace udp
{
  class UDPSender
  {
  public:
    // TODO specify default
    UDPSender(boost::asio::thread_pool& pool, const unsigned short& localPort);

    ~UDPSender();

    void stop();

    void ready();

    void send(const std::string& toSend,
              boost::asio::ip::udp::endpoint whereToSend);

    void startReceive();

    void handleReceive(const boost::system::error_code& error,
                       std::size_t bytes_transferred,
                       const boost::asio::ip::udp::endpoint& whereFrom);

    boost::signals2::scoped_connection registerReceiver(
      std::function<void(boost::asio::ip::udp::endpoint, std::string)> handler);

    void handleSend(const boost::system::error_code& error,
                    const std::size_t& bytes_transferred);

    std::shared_ptr<boost::asio::io_context> m_pCtx;
    boost::optional<boost::asio::io_context::work> m_optCork;
    std::shared_ptr<std::thread> m_pCtxThread;
    boost::asio::thread_pool& m_pool;
    boost::asio::ip::udp::endpoint m_localEndpoint;
    boost::asio::ip::udp::socket m_socket;
    boost::array<char, 128> m_recvBuffer;
    boost::signals2::signal<void(boost::asio::ip::udp::endpoint, std::string)>
      m_receiveSignal;
    AutoConsumedQueueEndpoint acq;
  };
} // namespace udp
