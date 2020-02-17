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
    void sendPing();

    std::shared_ptr<boost::asio::io_context> m_pCtx;
    boost::optional<boost::asio::io_context::work> m_optCork;
    boost::asio::steady_timer m_timer;
    tcp::TcpServer m_server;
    std::vector<boost::signals2::connection> m_connections;
    std::map<int, GameManager> m_gameManagers;
    std::thread m_iocThread;
  };
} // namespace referee

#endif
