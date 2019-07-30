#define BOOST_TEST_MODULE LoggerTest

#include <iostream>

#include <boost/filesystem.hpp>
#include <boost/test/unit_test.hpp>

#include "LoggerLib/Logger.hpp"

struct msg
{
  std::string from;
  std::string msg;
};

BOOST_AUTO_TEST_CASE(LOGGER_TEST)
{
  auto logPath = logger::getLogPath();

  std::vector<msg> msgs{{"GraphLib", "Unable to construct graph"},
                        {"TCP", "No connections available"},
                        {"TCP", "Send Msg"}};

  logger::logError(msgs[0].from, msgs[0].msg);
  logger::logWarning(msgs[1].from, msgs[1].msg);
  logger::logInfo(msgs[2].from, msgs[2].msg);

  std::string line;
  std::ifstream fin(logPath);
  while (std::getline(fin, line))
  {
    //line.find()
  }

  boost::filesystem::remove(logPath);
}
