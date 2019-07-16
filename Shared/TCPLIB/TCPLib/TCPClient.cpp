#include "TCPClient.hpp"

#include <iostream>

#include <boost/asio/write.hpp>

#include "TCPTools.hpp"

tcp::TcpClient::TcpClient(std::string hostname,
                          std::string port,
                          boost::asio::thread_pool& pool,
                          std::function<msg::BaseMsg(std::string)> parser)
  : m_ctx(),
    m_optCork(m_ctx),
    m_ctxThread([m_ctx = &m_ctx]() { m_ctx->run(); }),
    m_threadPool(pool),
    m_socket(m_ctx),
    m_inputBuffer(1024),
    m_parser(parser),
    m_handlers(),
    m_acq([m_threadPool = &m_threadPool,
           m_parser = &m_parser,
           m_handlers = &m_handlers](std::string& input) {
      static std::string inputBuffer;
      auto msg = tcp::getNextStringMessage(inputBuffer, input);
      if (!msg) return;
      boost::asio::post(*m_threadPool, [msg, m_parser, m_handlers]() {
        auto pMsg = std::make_shared<msg::BaseMsg>((*m_parser)(msg.get()));
        m_handlers->find(pMsg->getName())->second(pMsg);
      });
    })
{
  boost::asio::ip::tcp::resolver r(m_ctx);

  startConnect(r.resolve(boost::asio::ip::tcp::resolver::query(
    boost::asio::ip::tcp::v4(), hostname, port)));
}

tcp::TcpClient::~TcpClient() {
  m_optCork = boost::none;
  m_ctxThread.join();
}

void tcp::TcpClient::registerHandler(
  const std::string& key,
  std::function<void(std::shared_ptr<msg::BaseMsg>)> handler)
{
  m_handlers[key] = handler;
}

void tcp::TcpClient::send(const std::string& message)
{
  auto toSend = std::make_shared<std::string>(tcp::getProcessedString(message));
  m_socket.async_write_some(boost::asio::buffer(*toSend, toSend.get()->size()),
                            [](const boost::system::error_code& ec, std::size_t) {
                              // TODO
                              if (!ec) return;
                              std::cout << "Failed to send" << std::endl;
                            });
}

void tcp::TcpClient::startConnect(
  boost::asio::ip::tcp::resolver::iterator endpoint_iter)
{
  if (endpoint_iter != boost::asio::ip::tcp::resolver::iterator())
  {
    std::cout << "Trying " << endpoint_iter->endpoint() << "...\n";

    // Start the asynchronous connect operation.
    m_socket.async_connect(
      endpoint_iter->endpoint(),
      [this, endpoint_iter](const boost::system::error_code& ec) {
        handleConnect(ec, endpoint_iter);
      });
  }
  else
  {
    // There are no more endpoints to try. Shut down the client.
    // stop();
    // TODO
    std::cout << "No more endpoints to try" << std::endl;
  }
}

void tcp::TcpClient::handleConnect(
  const boost::system::error_code& ec,
  boost::asio::ip::tcp::resolver::results_type::iterator endpoint_iter)
{
  // The async_connect() function automatically opens the socket at the start
  // of the asynchronous operation. If the socket is closed at this time then
  // the timeout handler must have run first.
  if (!m_socket.is_open())
  {
    std::cout << "Connect timed out\n";

    // Try the next available endpoint.
    startConnect(++endpoint_iter);
  }

  // Check if the connect operation failed before the deadline expired.
  else if (ec)
  {
    std::cout << "Connect error: " << ec.message() << "\n";

    // We need to close the socket used in the previous connection attempt
    // before starting a new one.
    m_socket.close();

    // Try the next available endpoint.
    startConnect(++endpoint_iter);
  }

  // Otherwise we have successfully established a connection.
  else
  {
    std::cout << "Connected to " << endpoint_iter->endpoint() << "\n";

    // Start the input actor.
    startRead();
  }
}

void tcp::TcpClient::startRead()
{
  m_socket.async_receive(
    boost::asio::buffer(m_inputBuffer),
    [this](const boost::system::error_code& ec, std::size_t bt) { handleRead(ec, bt); });
}

void tcp::TcpClient::handleRead(const boost::system::error_code& ec, std::size_t bt)
{
  std::cout << "read some" << bt << std::endl;
  if (!ec)
  {
    // Extract the newline-delimited message from the buffer.
    m_acq.addBytes(m_inputBuffer);
    // Empty messages are heartbeats and so ignored.

    startRead();
  }
  else
  {
    std::cout << "Error on receive: " << ec.message() << "\n";

    // stop(); // TODO
  }
}
