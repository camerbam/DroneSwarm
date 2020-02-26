#ifndef GROUND_STATION_CONTROLLER_HPP
#define GROUND_STATION_CONTROLLER_HPP

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
  class GroundStationController
  {
  public:
    GroundStationController(std::string host, std::string port);

    ~GroundStationController();

    void start();

  private:
    void createRefereeMsgHandlers();
    void createDroneMsgHandlers(std::shared_ptr<tcp::TcpConnection> drone);
    void assignTargets();

    std::condition_variable m_cv;
    std::mutex m_m;
    int m_gameId;
    std::vector<boost::signals2::scoped_connection> m_connections;
    std::vector<msg::TargetMsg> m_targets;
    std::vector<msg::TargetMsg> m_assignedTargets;
    tcp::TcpServer m_droneServer;
    std::vector<std::shared_ptr<tcp::TcpConnection>> m_idleDrones;
    std::vector<std::shared_ptr<tcp::TcpConnection>> m_busyDrones;
    tcp::TcpClient m_toReferee;
    logger::MonitorLogger m_logger;
  };
} // namespace referee

#endif
