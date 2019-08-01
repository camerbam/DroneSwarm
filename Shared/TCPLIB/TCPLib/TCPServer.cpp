#include "TCPServer.hpp"

#include <iostream>

#include <boost/asio.hpp>
#include <boost/asio/write.hpp>

#include "MsgLib/BaseMsg.hpp"
#include "TCPTools.hpp"

std::shared_ptr<tcp::TcpServer::TcpConnection> tcp::TcpServer::TcpConnection::
  create(boost::asio::io_context& ctx, int id, boost::asio::thread_pool& pool)
{
  return std::shared_ptr<tcp::TcpServer::TcpConnection>(
    new tcp::TcpServer::TcpConnection(ctx, id, pool));
}

tcp::TcpServer::TcpConnection::TcpConnection(boost::asio::io_context& ctx,
                                             int id,
                                             boost::asio::thread_pool& pool)
  : m_socket(ctx),
    m_id(id),
    m_closedSignal(),
    m_threadPool(pool),
    m_inputBuffer(1024),
    m_handlers(),
    m_acq(std::function<void(std::string)>(
      [ m_threadPool = &m_threadPool,
        m_handlers = &m_handlers ](std::string input) {
        while (true)
        {
          auto optMsg = tcp::getNextStringMessage(input);
          if (!optMsg) return;
          boost::asio::post(*m_threadPool, [optMsg, &m_handlers]() {
            msg::BaseMsg receivedMsg;
            auto msg = optMsg.get();
            auto format = msg::getMsgFormat(msg);
            if (!msg::parseString(receivedMsg, msg, format))
            {
              std::cout << "Could not parse msg" << std::endl;
              return;
            }
            auto handle = m_handlers->get(receivedMsg.type());
            if (!handle) std::cout << "Received unknown message" << std::endl;
            handle->execute(receivedMsg.msg(), format);
          });
        }
      }))
{
}

boost::asio::ip::tcp::socket& tcp::TcpServer::TcpConnection::socket()
{
  return m_socket;
}

void tcp::TcpServer::TcpConnection::startRead()
{
  m_socket.async_read_some(
    boost::asio::buffer(m_inputBuffer),
    [this](const boost::system::error_code& ec, std::size_t bytes_transferred) {
      handleRead(ec, bytes_transferred);
    });
}

void tcp::TcpServer::TcpConnection::ready()
{
  m_acq.ready();
}

void tcp::TcpServer::TcpConnection::handleWrite(
  const boost::system::error_code& ec, size_t)
{
  if (ec) m_closedSignal(m_id);
}

void tcp::TcpServer::TcpConnection::close()
{
  m_socket.close();
}

void tcp::TcpServer::TcpConnection::handleRead(
  const boost::system::error_code& ec, std::size_t bytes_transferred)
{
  if (!ec)
  {
    std::string toAdd(
      m_inputBuffer.begin(), m_inputBuffer.begin() + bytes_transferred);
    m_acq.add(toAdd);
    m_inputBuffer.clear();
    m_inputBuffer.resize(1024);

    startRead();
  }
  else
  {
    std::cout << "Error on receive: " << ec.message() << "\n";
    m_closedSignal(m_id);
  }
}

void tcp::TcpServer::TcpConnection::registerDisconnect(
  boost::signals2::signal<void(int)>::slot_type slot)
{
  m_closedSignal.connect(slot);
}

tcp::TcpServer::TcpServer(unsigned short port, boost::asio::thread_pool& pool)
  : m_ctx(),
    m_optCork(m_ctx),
    m_iocThread([m_ctx = &m_ctx]() { m_ctx->run(); }),
    m_pAcceptor(
      m_ctx, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
    m_connections(),
    m_threadPool(pool)
{
  startAccept();
}

tcp::TcpServer::~TcpServer()
{
  m_optCork = boost::none;
  m_iocThread.join();
}

void tcp::TcpServer::startAccept()
{
  static int m_nextId = 0;
  std::shared_ptr<TcpConnection> newConnection = TcpConnection::create(
    m_pAcceptor.get_io_service(), m_nextId++, m_threadPool);
  m_pAcceptor.async_accept(
    newConnection->socket(),
    [&, newConnection](auto ec) { handleAccept(newConnection, ec); });
}

void tcp::TcpServer::handleAccept(std::shared_ptr<TcpConnection> newConnection,
                                  const boost::system::error_code& error)
{
  if (!error)
  {
    std::cout << "connected" << std::endl;
    newConnection->startRead();
    m_connections[newConnection->getId()] = newConnection;
    newConnection->registerDisconnect([m_connections = &m_connections](int id) {
      m_connections->erase(id);
      std::cout << "erased" << id << std::endl;
    });
    m_connectionHandler(newConnection);
  }
  startAccept();
}

void tcp::TcpServer::close()
{
  m_optCork = boost::none;
  m_ctx.stop();
}
