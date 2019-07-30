#include "Logger.hpp"

#include <ctime>
#include <fstream>
#include <iomanip>
#include <mutex>
#include <sstream>

#include <boost/dll/runtime_symbol_info.hpp>
#include <boost/filesystem.hpp>

#include "ACQLib/ACQ.hpp"

namespace
{
  std::string getLogPath()
  {
    auto basePath =
      boost::dll::program_location().replace_extension("").string();
    if (boost::filesystem::exists(basePath + ".txt")) return basePath + ".txt";
    for (size_t i = 1; i < 10000; i++)
    {
      auto tempPath = basePath + std::to_string(i) + ".txt";
      if (boost::filesystem::exists(tempPath)) return tempPath;
    }
    throw std::exception("Could not create log file");
  }

  std::string getString(const std::string& component, const std::string& msg)
  {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X")
       << std::setw(20) << msg << std::endl;
    return ss.str();
  }

  const std::string n_logPath = getLogPath();
  std::mutex n_logMutex;

  void logMsg(const std::string& kind,
              const std::string& component,
              const std::string& msg)
  {
    auto toPrint = getString(component, msg);
    std::lock_guard<std::mutex> lock(n_logMutex);
    std::ofstream fout(n_logPath, std::ofstream::app);
    fout << kind << ": " << getString << std::endl;
  }
} // namespace

void logger::logError(const std::string& component, const std::string& msg)
{
  logMsg("ERROR", component, msg);
}

void logger::logWarning(const std::string& component, const std::string& msg)
{
  logMsg("WARNING", component, msg);
}

void logger::logInfo(const std::string& component, const std::string& msg)
{
  logMsg("Info", component, msg);
}

std::string logger::getLogPath()
{
  return n_logPath;
}
