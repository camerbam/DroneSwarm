#ifndef DRONE_SIMULATOR_CONTROLLER_HPP
#define DRONE_SIMULATOR_CONTROLLER_HPP

#include <string>

#include <boost/asio/thread_pool.hpp>

#include "UDPLib/UDPSender.hpp"

class DroneController
{
public:
  DroneController(const unsigned short& port);

  void sendMsg(std::string msg);

private:
  boost::asio::thread_pool m_pool;
  udp::UDPSender m_sender;
  boost::asio::ip::udp::endpoint m_remoteEndpoint;
};

#endif