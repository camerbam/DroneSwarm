#include <string>

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/optional.hpp>
#include <boost/signals2.hpp>

namespace udp
{
  class UDPServer
  {
  public:
    UDPServer(const unsigned short& port, boost::asio::thread_pool& pool, bool isReady);

    ~UDPServer();

    void stop();

    void ready();

    void send(const std::string& toSend);

    void startReceive();

    void handleReceive(const boost::system::error_code& error,
                       std::size_t /*bytes_transferred*/);

    boost::signals2::scoped_connection registerReceiver(std::function<void(std::string)> handler);

    void handleSend(std::shared_ptr<std::string> /*message*/,
                    const boost::system::error_code& /*error*/,
                    std::size_t /*bytes_transferred*/);

    std::shared_ptr<boost::asio::io_context> m_pCtx;
    boost::optional<boost::asio::io_context::work> m_optCork;
    std::shared_ptr<std::thread> m_pCtxThread;
    boost::asio::ip::udp::socket m_socket;
    boost::asio::thread_pool& m_pool;
    boost::asio::ip::udp::endpoint m_remoteEndpoint;
    boost::array<char, 1> m_recvBuffer;
    boost::signals2::signal<void(std::string)> m_receiveSignal;
  };
} // namespace udp
