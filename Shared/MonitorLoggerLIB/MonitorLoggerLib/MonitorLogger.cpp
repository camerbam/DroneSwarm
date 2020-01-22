#include "MonitorLogger.hpp"

#include <iomanip>

#include <boost/asio/ip/host_name.hpp>

#include "MsgLib/StringMsg.hpp"
#include "LoggerLib/Logger.hpp"

namespace
{
  std::string getTime()
  {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
    return ss.str();
  }

  void logMsg(tcp::TcpClient& m_client,
              const std::function<void(const std::string& component,
                                       const std::string& msg)>& log,
              const std::string& kind,
              const std::string& component,
              const std::string& msg)
  {
    std::stringstream ss;
    ss << std::left << std::setw(8) << kind << std::setw(20) << component
       << ": " << getTime() << ": " << msg;
    auto str = ss.str();
    log(component, str);
    if (m_client.isConnected()) 
    {
      msg::StringMsg msg(str);
      m_client.send(msg);
    }
  }
} // namespace

logger::MonitorLogger::MonitorLogger(const std::string& name,
                                     const std::string& port)
  : m_client(boost::asio::ip::host_name(), port, msg::FORMAT::PROTOBUF)
{
  m_client.ready();
  msg::StringMsg msg(name);
  m_client.send(msg);
}

void logger::MonitorLogger::logError(const std::string& component,
                                     const std::string& msg)
{
  logMsg(m_client, logger::logError, "ERROR", component, msg);
}

void logger::MonitorLogger::logWarning(const std::string& component,
                                       const std::string& msg)
{
  logMsg(m_client, logger::logWarning, "WARNING", component, msg);
}

void logger::MonitorLogger::logInfo(const std::string& component,
                                    const std::string& msg)
{
  logMsg(m_client, logger::logInfo, "Info", component, msg);
}
