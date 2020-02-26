#ifndef PRETEST_CONTROLLER_HPP
#define PRETEST_CONTROLLER_HPP

#include <condition_variable>
#include <mutex>
#include <vector>

#include <boost/signals2/connection.hpp>

#include "MonitorLoggerLib/MonitorLogger.hpp"
#include "MsgLib/TargetMsg.hpp"
#include "TCPLib/TCPClient.hpp"
#include "TCPLib/TCPConnection.hpp"
#include "TCPLib/TCPServer.hpp"

namespace ground
{
  class PretestController
  {
  public:
    PretestController(std::string host, std::string port);

    ~PretestController();

  private:
    void createRefereeMsgHandlers();

    bool m_pinged;
    std::vector<std::condition_variable> m_cvs;
    std::vector<std::mutex> m_ms;
    std::vector<boost::signals2::scoped_connection> m_connections;
    tcp::TcpClient m_toReferee;
  };
} // namespace referee

#endif
