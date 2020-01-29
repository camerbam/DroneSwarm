#include "UDPCommunicatorReliable.hpp"

#include "Response.hpp"

#include <boost/date_time.hpp>

udp::UDPCommunicatorReliable::UDPCommunicatorReliable()
  : m_sender(), m_lastSent(), m_sending(), m_running(true), m_pPingThread()
{
}

udp::UDPCommunicatorReliable::~UDPCommunicatorReliable()
{
  m_running = false;
  if (m_pPingThread) m_pPingThread->join();
}

udp::Response udp::UDPCommunicatorReliable::sendMessage(
  const std::string& toSend,
  const boost::asio::ip::udp::endpoint& remoteEndpoint,
  const boost::posix_time::time_duration& timeout,
  std::function<bool(const std::string& msg)> validator,
  size_t tries)
{
  if (!tries) tries = 1;
  udp::Response response;
  for (size_t i = 0; i < tries; i++)
  {
    m_lastSent = boost::posix_time::microsec_clock::local_time();
    std::lock_guard<std::mutex> lock(m_sending);
    m_sender.sendMessage(toSend, remoteEndpoint);
    response = m_sender.receiveMessage(timeout);
    if (response.didSucceed())
    {
      if (!validator) return response;
      if (validator(response.getMessage())) return response;
    }
  }
  return response;
}

void udp::UDPCommunicatorReliable::startPing(
  std::function<void()> pingFunction,
  const boost::posix_time::time_duration& pingTime)
{
  m_lastSent = boost::posix_time::microsec_clock::local_time();
  m_pPingThread = std::make_shared<std::thread>([&, pingFunction, pingTime]() {
    while (m_running)
    {
      std::this_thread::sleep_for(std::chrono::milliseconds(2));
      if (boost::posix_time::microsec_clock::local_time() >
          m_lastSent + pingTime)
      {
        if (pingFunction) pingFunction();
      }
    }
  });
}
