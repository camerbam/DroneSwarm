#include "UDPCommunicator.hpp"

#include <iostream>

#include "Response.hpp"

udp::UDPCommunicator::UDPCommunicator()
  : m_ctx(),
    m_optCork(m_ctx),
    m_socket(m_ctx),
    m_ctxThread([m_ctx = &m_ctx]() { m_ctx->run(); })
{
  m_socket.open(boost::asio::ip::udp::v4());
}

udp::UDPCommunicator::UDPCommunicator(const short localPort)
  : m_ctx(),
    m_optCork(m_ctx),
    m_socket(
      m_ctx,
      boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), localPort)),
    m_ctxThread([m_ctx = &m_ctx]() { m_ctx->run(); })
{
}

udp::UDPCommunicator::~UDPCommunicator()
{
  m_optCork = boost::none;
  m_ctx.stop();
  m_ctxThread.join();
}

size_t udp::UDPCommunicator::sendMessage(
  const std::string& toSend,
  const boost::asio::ip::udp::endpoint& remoteEndpoint)
{
  return m_socket.send_to(boost::asio::buffer(toSend), remoteEndpoint);
}

udp::Response udp::UDPCommunicator::receiveMessage(
  const boost::posix_time::time_duration& timeout)
{
  std::string receiveBuffer(256, '0');
  boost::asio::ip::udp::endpoint endpoint;
  udp::Response response;
  auto start = boost::posix_time::microsec_clock::local_time();

  while (boost::posix_time::microsec_clock::local_time() < start + timeout)
  {
    if (m_socket.available())
    {
#ifdef __linux__
      auto bytes =
        m_socket.receive_from(boost::asio::buffer(receiveBuffer), endpoint);
#else
      boost::system::error_code ec;
      auto bytes = m_socket.receive_from(
        boost::asio::buffer(receiveBuffer), endpoint, 0, ec);
      if (ec.value())
      {
        response.setDidSucceed(false);
        response.setEndpoint(endpoint);
        response.setMessage(ec.message());
        return response;
      }
#endif
      response.setDidSucceed(true);
      response.setEndpoint(endpoint);
      response.setMessage(receiveBuffer.substr(0, bytes));
      return response;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
  response.setDidSucceed(false);
  response.setMessage("Receive Message timed out");
  return response;
}
