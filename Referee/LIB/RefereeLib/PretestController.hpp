#ifndef PRETEST_CONTROLLER_HPP
#define PRETEST_CONTROLLER_HPP

#include <map>
#include <condition_variable>
#include <mutex>

#include "GameManager.hpp"
#include "TCPLib/TCPServer.hpp"

namespace referee
{
  enum class MSG_STATUS
  {
    WAITING,
    RECEIVED,
    SEND_AGAIN
  };

  class PretestController
  {
  public:
    PretestController(unsigned short port, msg::FORMAT format, size_t pretest);

    ~PretestController();

    int execute();

  private:
    void pretest1();
    void pretest2();

    std::condition_variable m_cv;
    std::mutex m_m;
    tcp::TcpServer m_server;
    std::vector<boost::signals2::connection> m_connections;
    std::map<std::string, MSG_STATUS> m_msgs;
  };
} // namespace referee

#endif
