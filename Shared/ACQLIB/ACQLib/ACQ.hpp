#ifndef AutoConsumedQueue_hpp
#define AutoConsumedQueue_hpp

#include <atomic>
#include <thread>
#include <mutex>

#include <boost/asio/io_context.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/optional.hpp>

class AutoConsumedQueue
{
public:
  AutoConsumedQueue(std::function<void(std::string&, std::mutex&)> handler);

  ~AutoConsumedQueue();

  void addBytes(std::vector<char>& buf);

  bool isConsuming() { return m_running; }

private:
  std::atomic<bool> m_running;
  std::function<void(std::string&, std::mutex&)> m_handler;
  boost::asio::io_context m_ctx;
  boost::optional<boost::asio::io_context::work> m_optCork;
  std::mutex m_bufferMutex;
  std::string m_buffer;
  std::thread m_runningThread;
};

#endif