#ifndef REGISTRY_HPP
#define REGISTRY_HPP

#include <memory>

#include <boost/optional.hpp>

#include <boost/asio/thread_pool.hpp>
#include <boost/filesystem/path.hpp>

class GlobalRegistry
{
public:
  static GlobalRegistry& getRegistry();

  ~GlobalRegistry();

  void postToThreadPool(std::function<void()> fn);
  void setThreadPoolCount(size_t count = 1);

  void setSpeedRatio(double speedRatio);
  double getSpeedRatio();

  void setBatteryDecaySpeed(double decaySpeed);
  double getBatteryDecaySpeed();


  bool parseConfig(const boost::filesystem::path& configPath);

private:
  GlobalRegistry();

  static GlobalRegistry m_instance;

  std::shared_ptr<boost::asio::thread_pool> m_pThreadPool;
  double m_speedRatio;
  double m_decaySpeed;
};

#endif
