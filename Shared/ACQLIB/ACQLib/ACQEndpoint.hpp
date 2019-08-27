#ifndef AutoConsumedQueueEndpoint_hpp
#define AutoConsumedQueueEndpoint_hpp

#include <atomic>
#include <thread>

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/udp.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/optional.hpp>

class AutoConsumedQueueEndpoint
{
public:
  AutoConsumedQueueEndpoint(
    std::function<void(boost::asio::ip::udp::endpoint, std::string)> handler,
    bool isReady = false);

  ~AutoConsumedQueueEndpoint();

  void add(const boost::asio::ip::udp::endpoint& endpoint,
           const std::string& input);

  bool isConsuming() { return m_running; }

  void ready();

private:
  std::atomic<bool> m_running;
  std::function<void(boost::asio::ip::udp::endpoint, std::string)> m_handler;
  std::shared_ptr<boost::asio::io_context> m_ctx;
  boost::optional<boost::asio::io_context::work> m_optCork;
  std::shared_ptr<std::thread> m_pRunningThread;
};

#endif