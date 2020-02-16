#include "TCPServer.hpp"

#include <iostream>

#include <boost/asio.hpp>
#include <boost/asio/write.hpp>

#include "MsgLib/BaseMsg.hpp"
#include "TCPConnection.hpp"
#include "TCPTools.hpp"

tcp::TcpServer::TcpServer(unsigned short port, msg::FORMAT format)
  : m_pCtx(std::make_shared<boost::asio::io_context>()),
    m_optCork(*m_pCtx),
    m_format(format),
    m_timer(*m_pCtx, boost::asio::chrono::seconds(1)),
    m_pAcceptor(
      *m_pCtx,
      boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
    m_connections(),
    m_connectionHandler(),
    m_iocThread([pCtx = m_pCtx]() { pCtx->run(); })
{
  startAccept();
}

tcp::TcpServer::~TcpServer()
{
  m_optCork = boost::none;
  if (m_iocThread.joinable()) m_iocThread.join();
}

void tcp::TcpServer::startAccept()
{
  auto pSocket = std::make_shared<boost::asio::ip::tcp::socket>(*m_pCtx);
  m_pAcceptor.async_accept(
    *pSocket, [&, pSocket](auto ec) { handleAccept(pSocket, ec); });
}

void tcp::TcpServer::handleAccept(
  std::shared_ptr<boost::asio::ip::tcp::socket> pSocket,
  const boost::system::error_code& error)
{
  if (!error)
  {
    static int m_nextId = 0;
    std::shared_ptr<TcpConnection> newConnection =
      TcpConnection::create(pSocket, m_nextId++, m_format);
    newConnection->startRead();
    m_connections[newConnection->getId()] = newConnection;
    newConnection->registerDisconnect([m_connections = &m_connections](int id) {
      m_connections->erase(id);
    });
    m_connectionHandler(newConnection);
  }
  startAccept();
}

void tcp::TcpServer::close()
{
  m_timer.cancel();
  m_optCork = boost::none;
  m_pCtx->stop();
}

void tcp::TcpServer::checkMsgs()
{
  m_timer.async_wait([this](const boost::system::error_code& e) {
    if (!e) checkMsgs();
  });
  auto now = std::chrono::steady_clock::now();
  for (auto m : m_connections)
    m.second->checkMsgs(now);
}
