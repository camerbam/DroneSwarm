#include "Response.hpp"

udp::Response::Response() : m_remoteEndpoint(), m_message(), m_didSucceed()
{
}

udp::Response::Response(const boost::asio::ip::udp::endpoint& remoteEndpoint,
                        const std::string& message,
                        const bool didSucceed)
  : m_remoteEndpoint(remoteEndpoint),
    m_message(message),
    m_didSucceed(didSucceed)
{
}

boost::asio::ip::udp::endpoint udp::Response::getEndpoint() const
{
  return m_remoteEndpoint;
}

void udp::Response::setEndpoint(const boost::asio::ip::udp::endpoint& endpoint)
{
  m_remoteEndpoint = endpoint;
}

bool udp::Response::didSucceed() const
{
  return m_didSucceed;
}

void udp::Response::setDidSucceed(const bool didSucceed)
{
  m_didSucceed = didSucceed;
}

std::string udp::Response::getMessage() const
{
  return m_message;
}

void udp::Response::setMessage(const std::string& message)
{
  m_message = message;
}
