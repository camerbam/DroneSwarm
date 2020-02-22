#ifndef REGISTRY_HPP
#define REGISTRY_HPP

#include <complex>
#include <memory>

#include <boost/optional.hpp>

#include <boost/asio/thread_pool.hpp>
#include <boost/filesystem/path.hpp>

#include "Target.hpp"

class GlobalRegistry
{
public:
  static void setRegistry();
  static void setRegistry(const boost::filesystem::path& p);
  static void setRegistry(const std::string& config);
  static void setRegistry(
    double speed,
    double decaySpeed,
    const std::vector<Target>& targets = {},
    bool skipLog = true,
    bool encrypted = false);

  static GlobalRegistry& getRegistry();

  ~GlobalRegistry();

  void postToThreadPool(std::function<void()> fn);

  double getSpeedRatio();

  double getBatteryDecaySpeed();

  const std::vector<Target>& getTargets();

  bool getSkipLog();

  bool getPrintLog();

  size_t getPretest();

  bool isEncypted();

private:
  GlobalRegistry();
  GlobalRegistry(const GlobalRegistry&) = delete;

  GlobalRegistry(const std::string& p);
  bool parseConfig(const std::string& config);

  static std::shared_ptr<GlobalRegistry> m_pInstance;

  std::shared_ptr<boost::asio::thread_pool> m_pThreadPool;
  double m_speedRatio;
  double m_decaySpeed;
  std::vector<Target> m_targets;
  bool m_skipLog;
  bool m_printLog;
  size_t m_pretest;
  bool m_encypted;
};

#endif
