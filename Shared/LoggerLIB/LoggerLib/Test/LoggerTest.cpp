#define BOOST_TEST_MODULE LoggerTest

#include <iostream>
#include <thread>

#include <boost/dll/runtime_symbol_info.hpp>
#include <boost/filesystem.hpp>
#include <boost/test/unit_test.hpp>

#include "LoggerLib/Logger.hpp"
#include "RegistryLib/Registry.hpp"

struct Msg
{
  std::string from;
  std::string msg;
};

BOOST_AUTO_TEST_CASE(LOGGER_TEST)
{
  GlobalRegistry::setRegistry(100, 20, {}, false);

  auto logPath = logger::getLogPath();
  auto logBoostPath = boost::filesystem::path(logPath);
  auto logName = logBoostPath.filename().replace_extension("").string();
  logName.erase(
    std::remove_if(logName.begin(),
                   logName.end(),
                   [](auto const& c) -> bool { return std::isdigit(c); }),
    logName.end());

  std::vector<Msg> msgs{{"GraphLib", "Unable to construct graph"},
                        {"TCP", "No connections available"},
                        {"TCP", "Send Msg"}};

  logger::logError(msgs[0].from, msgs[0].msg);
  logger::logWarning(msgs[1].from, msgs[1].msg);
  logger::logInfo(msgs[2].from, msgs[2].msg);

  // TODO This is brittle
  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  std::string line;
  std::ifstream fin(logPath);
  for (size_t i = 0; i < msgs.size(); i++)
  {
    std::getline(fin, line);
    auto from = line.find(msgs[i].from);
    BOOST_CHECK(from != std::string::npos);
    auto msg = line.find(msgs[i].msg, from);
    BOOST_CHECK(msg != std::string::npos);
  }
 
  for (boost::filesystem::directory_iterator iter(
          boost::dll::program_location().remove_filename());
       iter != boost::filesystem::directory_iterator();
       iter++)
  {
    if (iter->path().string().find(logName) != std::string::npos &&
        iter->path().string() != logPath && iter->path().extension() == ".txt")
      boost::filesystem::remove(*iter);
  }
}
