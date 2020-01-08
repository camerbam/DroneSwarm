#ifndef REGISTRY_HPP
#define REGISTRY_HPP

#include <memory>

#include <boost/asio/thread_pool.hpp>

class GlobalRegistry
{
public:
  ~GlobalRegistry();

  boost::asio::thread_pool& getThreadPool();

  static GlobalRegistry& getRegistry();

private:
  GlobalRegistry();

  boost::asio::thread_pool m_threadPool;
};

#endif
