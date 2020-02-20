#include "StatusManager.hpp"

#include <cctype>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

#include "UDPLib/Response.hpp"
#include "UDPLib/UDPCommunicator.hpp"

drone::StatusManager::StatusManager(const std::string& ports)
  : m_pThread(), m_ports()
{
  parsePorts(ports);
  m_pThread = std::make_shared<std::thread>([this]() {
    udp::UDPCommunicator communicator(8890);
    while (m_running)
    {
      auto response = communicator.receiveMessage();
      if (!response.didSucceed()) continue;
      auto end = response.getEndpoint().address().to_string();
      auto spot = end.find_last_of(".");
      communicator.sendMessage(
        response.getMessage(),
        boost::asio::ip::udp::endpoint(
          boost::asio::ip::address::from_string("127.0.0.1"), m_ports[end]));
    }
  });
}

drone::StatusManager::~StatusManager()
{
  m_running = false;
  if (m_pThread) m_pThread->join();
}

void drone::StatusManager::parsePorts(
  std::string ports)
{
  std::vector<std::string> combinations;
  boost::split(combinations, ports, boost::algorithm::is_space());

  for (auto&& str : combinations)
  {
    auto spot = str.find(":");
    m_ports.emplace(
      str.substr(0, spot), (unsigned short)stoul(str.substr(spot + 1)));
  }
}
