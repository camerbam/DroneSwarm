#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/optional.hpp>
#include <iostream>

class UDPClient
{
public:
  UDPClient(const std::string& host, const std::string& port, bool isReady = true)
    : m_pCtx(std::make_shared<boost::asio::io_context>()),
      m_optCork(*m_pCtx),
      m_pCtxThread(),
      m_socket(*m_pCtx)
  {
    boost::asio::ip::udp::resolver resolver(*m_pCtx);
    m_remoteEndpoint =
      *resolver.resolve(boost::asio::ip::udp::v4(), host, port).begin();

    boost::asio::ip::udp::socket socket(*m_pCtx);
    socket.open(boost::asio::ip::udp::v4());

    if (isReady) ready();
  }

  void ready()
  {
    m_pCtxThread =
      std::make_shared<std::thread>([m_pCtx = m_pCtx]() { m_pCtx->run(); });
  }

  void send(const std::string& toSend)
  {
    m_socket.send(boost::asio::buffer(toSend));
  }

  void receive()
  {
    boost::array<char, 128> recv_buf;
    boost::asio::ip::udp::endpoint sender_endpoint;
    size_t len = m_socket.receive_from(
      boost::asio::buffer(m_recvBuffer), m_remoteEndpoint);

    std::cout << recv_buf.data() << std::endl;

  }
private:


  std::shared_ptr<boost::asio::io_context> m_pCtx;
  boost::optional<boost::asio::io_context::work> m_optCork;
  std::shared_ptr<std::thread> m_pCtxThread;
  boost::asio::ip::udp::socket m_socket;
  boost::asio::ip::udp::endpoint m_remoteEndpoint;
  boost::array<char, 1> m_recvBuffer;
};
