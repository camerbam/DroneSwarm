#ifndef DRONE_MANAGER_HPP
#define DRONE_MANAGER_HPP

#include <queue>
#include <string>

#include "DroneManagerLib/DroneController.hpp"
#include "MonitorLoggerLib/MonitorLogger.hpp"
#include "TCPLib/TCPClient.hpp"

namespace msg
{
  class Point;
}

namespace drone
{
  std::queue<messages::Message_t> createFlightPath(
    double x, double y, const std::vector<msg::Point>& points); // TODO Unit test

  class DroneManager
  {
  public:
    DroneManager(const std::string& ipAddress,
                 const std::string& serverPort,
                 const std::string& monitorPort);

    ~DroneManager();

  private:
    void registerHandlers();

    void startMessages();

    DroneController m_controller;
    tcp::TcpClient m_client;
    std::mutex m_pathMutex;
    std::queue<messages::Message_t> m_flightPath;
    std::vector<msg::Point> m_points;
    std::vector<boost::signals2::scoped_connection> m_connections;
    logger::MonitorLogger m_logger;
    std::shared_ptr<std::thread> m_sendThread;
    bool m_toQuit;
    double m_zConfig;
  };
} // namespace drone

#endif
