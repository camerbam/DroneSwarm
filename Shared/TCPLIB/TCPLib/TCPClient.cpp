#include "TCPClient.hpp"

#include <iostream>

#include <boost/asio/write.hpp>

#include "TCPTools.hpp"

tcp::TcpClient::~TcpClient()
{
  m_optCork = boost::none;
  m_ctxThread.join();
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

void tcp::TcpClient::handleWrite(
  const boost::system::error_code& ec, size_t bt)
{
  std::cout << "wrote" << bt << std::endl;
  //if (ec) m_closedSignal(m_id); // TODO What to do here
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
  std::cout << "read some" << bt << std::endl;
  if (!ec)
  {
    std::vector<char> toAdd(m_inputBuffer.begin(), m_inputBuffer.begin() + bt);
    m_acq.addBytes(toAdd);
    m_inputBuffer.clear();
    m_inputBuffer.resize(1024);

    startRead();
  }
  else
  {
    std::cout << "Error on receive: " << ec.message() << "\n";

    // stop(); // TODO
  }
}
