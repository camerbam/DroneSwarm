#include "TCPClient.hpp"

#include <iostream>

#include <boost/asio/write.hpp>

#include "RegistryLib/Registry.hpp"
#include "TCPTools.hpp"

tcp::TcpClient::TcpClient(std::string hostname,
                          std::string port,
                          msg::FORMAT format)
  : m_ctx(),
    m_optCork(m_ctx),
    m_format(format),
    m_ctxThread([m_ctx = &m_ctx]() { m_ctx->run(); }),
    m_socket(m_ctx),
    m_inputBuffer(1024),
    m_handlers(std::make_shared<tcp::HandlerMap>()),
    m_acq(std::function<void(std::string)>(
      [ m_handlers = m_handlers, format = m_format ](std::string input) {
        while (true)
        {
          auto optMsg = tcp::getNextStringMessage(input);
          if (!optMsg) return;
          GlobalRegistry::getRegistry().postToThreadPool(
            [optMsg, m_handlers, format]() {
              msg::BaseMsg receivedMsg;
              auto msg = optMsg.get();
              if (!parseString(receivedMsg, msg, format))
              {
                std::cout << "Could not parse msg" << std::endl;
                return;
              }
              auto handle = m_handlers->get(receivedMsg.type());
              if (!handle)
              {
                std::cout << "1Received unknown message" << std::endl;
                return;
              }
              handle->execute(receivedMsg.msg(), format);
            });
        }
      }))
{
  boost::asio::ip::tcp::resolver r(m_ctx);

  startConnect(r.resolve(boost::asio::ip::tcp::resolver::query(
    boost::asio::ip::tcp::v4(), hostname, port)));
}

tcp::TcpClient::~TcpClient()
{
  m_optCork = boost::none;
  if (m_ctxThread.joinable()) m_ctxThread.join();
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
    std::cout << "No more endpoints to try" << std::endl;
    close();
  }
}

void tcp::TcpClient::handleWrite(const boost::system::error_code&, size_t)
{
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
  m_socket.async_receive(boost::asio::buffer(m_inputBuffer),
                         [this](const boost::system::error_code& ec,
                                std::size_t bt) { handleRead(ec, bt); });
}

void tcp::TcpClient::handleRead(const boost::system::error_code& ec,
                                std::size_t bt)
{
  if (!ec)
  {
    std::string toAdd(m_inputBuffer.begin(), m_inputBuffer.begin() + bt);
    m_acq.add(toAdd);
    m_inputBuffer.clear();
    m_inputBuffer.resize(1024);

    startRead();
  }
  else
  {
    std::cout << "Error on receive: " << ec.message() << "\n";

    close();
  }
}

void tcp::TcpClient::close()
{
  m_optCork = boost::none;
  m_ctx.stop();
}

void tcp::TcpClient::ready()
{
  m_acq.ready();
}

bool tcp::TcpClient::isConnected()
{
  return m_socket.is_open();
}
