#ifndef REFEREE_CONTROLLER_HPP
#define REFEREE_CONTROLLER_HPP

#include <map>

#include "GameManager.hpp"
#include "TCPLib/TCPServer.hpp"

namespace referee
{
  class RefereeController
  {
  public:
    RefereeController(unsigned short port, msg::FORMAT format);

    ~RefereeController();

  private:
    tcp::TcpServer m_server;
    std::vector<boost::signals2::connection> m_connections;
    std::map<int, GameManager> m_gameManagers;
  };
} // namespace referee

#endif
