#include "TCPServer.hpp"

#include <iostream> //TODO Remove this

#include <boost/asio.hpp>
#include <boost/asio/write.hpp>

#include "TCPTools.hpp"

std::shared_ptr<tcp::TcpServer::TcpConnection> tcp::TcpServer::TcpConnection::
  create(boost::asio::io_context& ctx,
         int id,
         boost::asio::thread_pool& pool,
         std::function<msg::BaseMsg(std::string)> parser,
         std::map<std::string,
                  std::function<void(std::shared_ptr<msg::BaseMsg>)>>& handlers)
{
  return std::shared_ptr<TcpServer::TcpConnection>(
    new TcpServer::TcpConnection(ctx, id, pool, parser, handlers));
}

boost::asio::ip::tcp::socket& tcp::TcpServer::TcpConnection::socket()
{
  return m_socket;
}

void tcp::TcpServer::TcpConnection::send(std::string message)
{
  auto pMessage = std::make_shared<std::string>(message);
  boost::asio::async_write(
    m_socket,
    boost::asio::buffer(*pMessage, pMessage.get()->size()),
    [this, pMessage](auto a, auto b) { this->handleWrite(a, b); });
}

tcp::TcpServer::TcpConnection::TcpConnection(
  boost::asio::io_context& ctx,
  int id,
  boost::asio::thread_pool& pool,
  std::function<msg::BaseMsg(std::string)> parser,
  std::map<std::string, std::function<void(std::shared_ptr<msg::BaseMsg>)>>&
    handlers)
  : m_socket(ctx),
    m_id(id),
    m_closedSignal(),
    m_threadPool(pool),
    m_parser(parser),
    m_inputBuffer(128),
    m_handlers(handlers),
    m_acq([m_threadPool = &m_threadPool,
           m_parser = &m_parser,
           m_handlers = &m_handlers](const std::string& input) {
      static std::string inputBuffer;
      auto msg = tcp::getNextStringMessage(inputBuffer, input);
      if (!msg) return;
      boost::asio::post(*m_threadPool, [msg, m_parser, m_handlers]() {
        auto pMsg = std::make_shared<msg::BaseMsg>((*m_parser)(msg.get()));
        m_handlers->find(pMsg->getName())->second(pMsg);
      });
    })
{
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

tcp::TcpServer::TcpServer(unsigned short port,
                          boost::asio::thread_pool& pool,
                          std::function<msg::BaseMsg(std::string)> parser)
  : m_ctx(),
    m_optCork(m_ctx),
    m_iocThread([m_ctx = &m_ctx]() { m_ctx->run(); }),
    m_pAcceptor(
      m_ctx, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
    m_connections(),
    m_threadPool(pool),
    m_parser(parser)
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
    m_connections[newConnection->getId()] = newConnection;
    newConnection->registerDisconnect([m_connections = &m_connections](int id) {
      m_connections->erase(id);
      std::cout << "erased" << id << std::endl;
    });
  }
  startAccept();
}

void tcp::TcpServer::registerHandler(
  const std::string& key,
  std::function<void(std::shared_ptr<msg::BaseMsg>)> handler)
{
  m_handlers[key] = handler;
}

void tcp::TcpServer::sendToAll(const std::string& message)
{
  for (auto&& connection : m_connections)
  {
    connection.second->send(message);
  }
}
