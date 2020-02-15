#define BOOST_TEST_MODULE MonitorLoggerTest

#include <condition_variable>
#include <thread>

#include <boost/dll/runtime_symbol_info.hpp>
#include <boost/filesystem.hpp>
#include <boost/test/unit_test.hpp>

#include "LoggerLib/Logger.hpp"
#include "MonitorLoggerLib/MonitorLogger.hpp"
#include "MsgLib/StringMsg.hpp"
#include "TCPLib/TCPServer.hpp"

std::shared_ptr<std::thread> startServer(size_t& msgsLeft)
{
  return std::make_shared<std::thread>([&msgsLeft]() {
    std::vector<boost::signals2::scoped_connection> connections;
    tcp::TcpServer server(13000);
    std::condition_variable cv;
    std::mutex mutex;

    auto connection =
      server.registerConnection([&connections, &server, &msgsLeft, &cv](
        std::shared_ptr<tcp::TcpConnection> pConnection) {
        connections.push_back(pConnection->registerHandler<msg::StringMsg>(
          [pConnection, &msgsLeft, &cv](msg::StringMsg, const std::string& msgId) {
            msgsLeft--;
            if (msgsLeft == 0)
            {
              pConnection->close();
              cv.notify_one();
            }
          }));
        pConnection->ready();
      });
    std::unique_lock<std::mutex> lk(mutex);
    cv.wait_for(lk, std::chrono::seconds(2));
    server.close();
  });
}

BOOST_AUTO_TEST_CASE(MONITOR_LOGGER_TEST)
{
  GlobalRegistry::setRegistry(1, 1);
  auto logPath = logger::getLogPath();
  auto logBoostPath = boost::filesystem::path(logPath);
  auto logName = logBoostPath.filename().replace_extension("").string();
  logName.erase(
    std::remove_if(logName.begin(),
                   logName.end(),
                   [](auto const& c) -> bool { return std::isdigit(c); }),
    logName.end());
  size_t msgsLeft = 4;
  auto t = startServer(msgsLeft);
  logger::MonitorLogger logger("localhost", "13000");

  logger.logError("test", "testLog");
  logger.logInfo("test", "testLog");
  logger.logWarning("test", "testLog");

  t->join();
  BOOST_CHECK(!msgsLeft);
  logger.logWarning("test", "testLog");

  for (boost::filesystem::directory_iterator iter(
         boost::dll::program_location().remove_filename());
       iter != boost::filesystem::directory_iterator();
       iter++)
  {
    if (iter->path().string().find(logName) != std::string::npos &&
        iter->path().extension() == ".txt")
      boost::filesystem::remove(*iter);
  }
}
