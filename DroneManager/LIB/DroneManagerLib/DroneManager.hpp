#ifndef DRONE_MANAGER_HPP
#define DRONE_MANAGER_HPP

#include <queue>
#include <string>
#include <condition_variable>

#include "DroneManagerLib/DroneController.hpp"
#include "MonitorLoggerLib/MonitorLogger.hpp"
#include "TCPLib/TCPClient.hpp"

namespace msg
{
  class TargetMsg;
}

namespace drone
{
  std::queue<messages::Message_t> createFlightPath(
    double x, double y, const std::vector<msg::TargetMsg>& targets);

  class DroneManager
  {
  public:
    DroneManager(const std::string& ipAddress,
                 const unsigned short& dronePort,
                 const std::string& serverPort,
                 const std::string& monitorPort,
                 int startingY);

    ~DroneManager();

  private:
    void registerHandlers();

    void startMessages();

    std::condition_variable m_cv;
    std::mutex m_m;
    logger::MonitorLogger m_logger;
    DroneController m_controller;
    tcp::TcpClient m_client;
    std::mutex m_pathMutex;
    std::queue<messages::Message_t> m_flightPath;
    std::vector<msg::TargetMsg> m_targets;
    std::vector<boost::signals2::scoped_connection> m_connections;
    std::shared_ptr<std::thread> m_sendThread;
    bool m_toQuit;
    double m_zConfig;
  };
} // namespace drone

#endif
