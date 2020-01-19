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
  static void setRegistry(const boost::filesystem::path& p);
  static void setRegistry(const std::string& config);
  static void setRegistry(
    double speed,
    double decaySpeed,
    const std::vector<Target>& targets = {},
    bool skipLog = true);

  static GlobalRegistry& getRegistry();

  ~GlobalRegistry();

  void postToThreadPool(std::function<void()> fn);

  double getSpeedRatio();

  double getBatteryDecaySpeed();

  const std::vector<Target>& getTargets();

  bool getSkipLog();

private:
  GlobalRegistry() = default;
  GlobalRegistry(const GlobalRegistry&) = delete;

  GlobalRegistry(const std::string& p);
  bool parseConfig(const std::string& config);

  static std::shared_ptr<GlobalRegistry> m_pInstance;

  std::shared_ptr<boost::asio::thread_pool> m_pThreadPool;
  double m_speedRatio;
  double m_decaySpeed;
  std::vector<Target> m_targets;
  bool m_skipLog;
};

#endif
