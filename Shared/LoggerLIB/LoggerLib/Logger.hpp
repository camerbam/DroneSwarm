#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>

namespace logger
{
  void logError(const std::string& component, const std::string& msg);
  void logWarning(const std::string& component, const std::string& msg);
  void logInfo(const std::string& component, const std::string& msg);
  std::string getLogPath();
}

#endif
