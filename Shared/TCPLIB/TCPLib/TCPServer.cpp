#include "TCPServer.hpp"

#include <iostream> //TODO Remove this

#include <boost/asio.hpp>
#include <boost/asio/write.hpp>

#include "TCPTools.hpp"

boost::asio::ip::tcp::socket& tcp::TcpServer::TcpConnection::socket()
{
  return m_socket;
}

void tcp::TcpServer::TcpConnection::send(std::string message)
{
  auto pMessage = std::make_shared<std::string>(tcp::getProcessedString(message));
  std::cout << "sending" << std::endl;
  m_socket.async_write_some(boost::asio::buffer(*pMessage, pMessage.get()->size()),
    [this, pMessage](auto a, auto b) { this->handleWrite(a, b); });
}

void tcp::TcpServer::TcpConnection::startRead()
{
  m_socket.async_read_some(
    boost::asio::buffer(m_inputBuffer),
    [this](const boost::system::error_code& ec, std::size_t bytes_transferred) {
      handleRead(ec, bytes_transferred);
    });
}

void tcp::TcpServer::TcpConnection::handleWrite(
  const boost::system::error_code& ec, size_t bt)
{
  std::cout << "wrote" << bt << std::endl;
  if (ec) m_closedSignal(m_id);
}

void tcp::TcpServer::TcpConnection::handleRead(
  const boost::system::error_code& ec, std::size_t bytes_transferred)
{
  if (!ec)
  {
    // Extract the newline-delimited message from the buffer.
    m_acq.addBytes(m_inputBuffer);

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

tcp::TcpServer::~TcpServer()
{
  m_optCork = boost::none;
  m_iocThread.join();
}

void tcp::TcpServer::startAccept()
{
  static int m_nextId = 0;
  std::shared_ptr<TcpConnection> newConnection =
    TcpConnection::create(m_pAcceptor.get_io_service(),
                          m_nextId++,
                          m_threadPool,
                          m_parser,
                          m_handlers);
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
  }
  startAccept();
}

void tcp::TcpServer::sendToAll(const std::string& message)
{
  for (auto&& connection : m_connections)
  {
    connection.second->send(message);
  }
}
