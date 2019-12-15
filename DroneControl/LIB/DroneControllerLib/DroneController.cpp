#include "DroneController.hpp"

#include <iostream>

DroneController::DroneController(const unsigned short& port)
  : m_pool(1),
    m_sender(m_pool, port),
    m_remoteEndpoint(
      boost::asio::ip::address_v4::from_string("192.168.10.1"), 8889)
{
  m_sender.registerReceiver(
    [](boost::asio::ip::udp::endpoint endpoint, std::string msg) {
      std::cout << msg << std::endl;
    });
}

void DroneController::sendMsg(std::string msg)
{
  m_sender.send(msg, m_remoteEndpoint);
}
