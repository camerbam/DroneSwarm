#include "Registry.hpp"

#include <fstream>

#include <boost/asio/post.hpp>
#include <boost/filesystem/operations.hpp>

#include <rapidjson/istreamwrapper.h>

#include "JSONLib/Deserialize.hpp"

GlobalRegistry::GlobalRegistry() : m_pThreadPool(), m_speedRatio(1), m_decaySpeed(2)
{
}

GlobalRegistry& GlobalRegistry::getRegistry()
{
  return m_instance;
}

GlobalRegistry::~GlobalRegistry()
{
  if (m_instance.m_pThreadPool) m_instance.m_pThreadPool->join();
}

void GlobalRegistry::postToThreadPool(std::function<void()> fn)
{
  if (!m_instance.m_pThreadPool) m_instance.setThreadPoolCount();
  boost::asio::post(*m_instance.m_pThreadPool, fn);
}

void GlobalRegistry::setThreadPoolCount(size_t count)
{
  if (!count) count = 1;
    m_instance.m_pThreadPool =
      std::make_shared<boost::asio::thread_pool>(count);
}

void GlobalRegistry::setSpeedRatio(double speedRatio)
{
  if (speedRatio < 1)
    m_instance.m_speedRatio = 1;
  else
    m_instance.m_speedRatio = speedRatio;
}

double GlobalRegistry::getSpeedRatio()
{
  return m_instance.m_speedRatio;
}

void GlobalRegistry::setBatteryDecaySpeed(double decaySpeed)
{
  if (decaySpeed < 1)
    m_instance.m_decaySpeed = 1;
  else
    m_instance.m_decaySpeed = decaySpeed;
}

double GlobalRegistry::getBatteryDecaySpeed()
{
  return m_instance.m_decaySpeed;
}

bool GlobalRegistry::parseConfig(const boost::filesystem::path& configPath)
{
  if (!boost::filesystem::exists(configPath)) return false;
  rapidjson::Document doc;
  std::ifstream t(configPath.string());
  rapidjson::IStreamWrapper isw{t};
  doc.ParseStream(isw);

  if (doc.IsNull()) return false;

  setSpeedRatio(json::getNumber(doc, "BatteryDecaySpeed"));

  return true;
}

GlobalRegistry GlobalRegistry::m_instance;
