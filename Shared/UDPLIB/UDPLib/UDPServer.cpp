#include "UDPServer.hpp"

#include <iostream>

udp::UDPServer::UDPServer(const unsigned short& port,
                          boost::asio::thread_pool& pool,
                          bool isReady)
  : m_pCtx(std::make_shared<boost::asio::io_context>()),
    m_optCork(*m_pCtx),
    m_pCtxThread(),
    m_pool(pool),
    m_socket(
      *m_pCtx, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port))
{
  if (isReady) ready();
  startReceive();
}

udp::UDPServer::~UDPServer()
{
  m_optCork = boost::none;
  m_pCtxThread->join();
}

void udp::UDPServer::stop()
{
  m_pCtx->stop();
}

void udp::UDPServer::ready()
{
  m_pCtxThread =
    std::make_shared<std::thread>([m_pCtx = m_pCtx]() { m_pCtx->run(); });
}

void udp::UDPServer::send(const std::string& toSend)
{
  std::shared_ptr<std::string> pToSend = std::make_shared<std::string>(toSend);

  m_socket.async_send_to(
    boost::asio::buffer(*pToSend),
    m_remoteEndpoint,
    [this, pToSend](auto ec, auto bt) { handleSend(pToSend, ec, bt); });
}

void udp::UDPServer::startReceive()
{
  m_socket.async_receive_from(
    boost::asio::buffer(m_recvBuffer),
    m_remoteEndpoint,
    boost::bind(&UDPServer::handleReceive,
                this,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
}

void udp::UDPServer::handleReceive(const boost::system::error_code& error,
                                   std::size_t /*bytes_transferred*/)
{
  std::cout << "a" << std::endl;
  if (!error || error == boost::asio::error::message_size)
  {
    m_receiveSignal(m_recvBuffer.data());
    startReceive();
  }
}

boost::signals2::scoped_connection udp::UDPServer::registerReceiver(
  std::function<void(std::string)> handler)
{
  auto poster = [this](std::string msg, std::function<void(std::string)> f) {
    boost::asio::post(m_pool, [msg, f]() { f(msg); });
  };
  boost::signals2::slot<void(std::string)> slot =
    [poster, handler](std::string msg) { poster(msg, handler); };
  return m_receiveSignal.connect(slot);
}

void udp::UDPServer::handleSend(std::shared_ptr<std::string> /*message*/,
                                const boost::system::error_code& /*error*/,
                                std::size_t /*bytes_transferred*/)
{
}
