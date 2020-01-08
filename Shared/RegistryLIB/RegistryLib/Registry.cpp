#include "Registry.hpp"

GlobalRegistry::GlobalRegistry() : m_threadPool(1)
{
}

GlobalRegistry::~GlobalRegistry()
{
  m_threadPool.join();
}

boost::asio::thread_pool& GlobalRegistry::getThreadPool()
{
  return m_threadPool;
}

GlobalRegistry& GlobalRegistry::getRegistry()
{
  static GlobalRegistry instance;
  return instance;
}
