#ifndef AutoConsumedQueue_hpp
#define AutoConsumedQueue_hpp

#include <atomic>
#include <thread>

#include <boost/asio/io_context.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/optional.hpp>

class AutoConsumedQueue
{
public:
  AutoConsumedQueue(std::function<void(std::string)> handler);

  ~AutoConsumedQueue();

  void add(const std::string& input);

  bool isConsuming() { return m_running; }

private:
  std::atomic<bool> m_running;
  std::function<void(std::string)> m_handler;
  std::shared_ptr<boost::asio::io_context> m_ctx;
  boost::optional<boost::asio::io_context::work> m_optCork;
  std::thread m_runningThread;
};

#endif