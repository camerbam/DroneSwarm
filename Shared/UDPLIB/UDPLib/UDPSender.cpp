#include "UDPSender.hpp"

#include <iostream>

udp::UDPSender::UDPSender(unsigned short& localPort,
                          boost::asio::thread_pool& pool)
  : m_pCtx(std::make_shared<boost::asio::io_context>()),
    m_optCork(*m_pCtx),
    m_pCtxThread(),
    m_pool(pool),
    m_localEndpoint(boost::asio::ip::udp::v4(), localPort),
    m_socket(*m_pCtx, m_localEndpoint),
    m_recvBuffer(),
    m_receiveSignal(),
    acq([this](boost::asio::ip::udp::endpoint endpoint, std::string received) {
      m_receiveSignal(endpoint, received);
    })
{
}

udp::UDPSender::~UDPSender()
{
  m_optCork = boost::none;
  m_pCtxThread->join();
}

void udp::UDPSender::stop()
{
  m_pCtx->stop();
}

void udp::UDPSender::ready()
{
  acq.ready();
  m_pCtxThread =
    std::make_shared<std::thread>([m_pCtx = m_pCtx]() { m_pCtx->run(); });
  startReceive();
}

void udp::UDPSender::send(const std::string& toSend,
                          boost::asio::ip::udp::endpoint whereToSend)
{
  std::shared_ptr<std::string> pToSend = std::make_shared<std::string>(toSend);
  m_socket.async_send_to(
    boost::asio::buffer(*pToSend),
    whereToSend,
    [this, pToSend](auto ec, auto bt) { handleSend(ec, bt); });
}

void udp::UDPSender::startReceive()
{
  boost::asio::ip::udp::endpoint receivedFrom;
  m_socket.async_receive_from(
    boost::asio::buffer(m_recvBuffer),
    receivedFrom,
    [this, &receivedFrom](
      const boost::system::error_code& error, std::size_t bytes_transferred) {
      handleReceive(error, bytes_transferred, receivedFrom);
    });
}

void udp::UDPSender::handleReceive(const boost::system::error_code& error,
                                   std::size_t bytes_transferred,
                                   const boost::asio::ip::udp::endpoint& whereFrom)
{
  if (!error || error == boost::asio::error::message_size)
  {
    acq.add(whereFrom, std::string(m_recvBuffer.data(), bytes_transferred));
    startReceive();
    return;
  }
  else if (error == boost::asio::error::connection_refused)
  {
    std::cout << "you might have sent a message that no one recieved..."
              << std::endl;
    startReceive();
  }
  std::cout << error.message() << std::endl;
}

boost::signals2::scoped_connection udp::UDPSender::registerReceiver(
  std::function<void(boost::asio::ip::udp::endpoint, std::string)> handler)
{
  auto poster = [this](
    std::string msg,
    boost::asio::ip::udp::endpoint endpoint,
    std::function<void(boost::asio::ip::udp::endpoint, std::string)> f) {
    boost::asio::post(m_pool, [msg, endpoint, f]() { f(endpoint, msg); });
  };

  boost::signals2::slot<void(boost::asio::ip::udp::endpoint, std::string)>
    slot = [poster, handler](
      boost::asio::ip::udp::endpoint endpoint, std::string msg) {
      poster(msg, endpoint, handler);
    };

  return m_receiveSignal.connect(slot);
}

void udp::UDPSender::handleSend(const boost::system::error_code& error,
                                const std::size_t& bytes_transferred)
{
  if (error) std::cout << error.message() << std::endl;
}
