#ifndef MONITOR_LOGGER_HPP
#define MONITOR_LOGGER_HPP

#include <string>

#include "TCPLib/TCPClient.hpp"

namespace logger
{
  class MonitorLogger
  {
  public:
    MonitorLogger(const std::string& name, const std::string& hostname, const std::string& port);

    void logError(const std::string& component, const std::string& msg);
    void logWarning(const std::string& component, const std::string& msg);
    void logInfo(const std::string& component, const std::string& msg);

  private:
    tcp::TcpClient m_client;
  };
}

#endif