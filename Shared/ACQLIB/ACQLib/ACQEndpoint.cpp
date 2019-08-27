#include "ACQEndpoint.hpp"

AutoConsumedQueueEndpoint::AutoConsumedQueueEndpoint(
  std::function<void(boost::asio::ip::udp::endpoint, std::string)> handler,
  bool isReady)
  : m_running(false),
    m_handler(handler),
    m_ctx(std::make_shared<boost::asio::io_context>()),
    m_optCork(*m_ctx),
    m_pRunningThread()
{
  if (isReady) ready();
}

AutoConsumedQueueEndpoint::~AutoConsumedQueueEndpoint()
{
  m_optCork = boost::none;
  if (m_pRunningThread) m_pRunningThread->join();
}

void AutoConsumedQueueEndpoint::add(
  const boost::asio::ip::udp::endpoint& endpoint, const std::string& input)
{
  m_running = true;
  m_ctx->post(
    [ m_handler = m_handler, m_running = &m_running, input, endpoint ] {
      m_handler(endpoint, input);
      *m_running = false;
    });
}

void AutoConsumedQueueEndpoint::ready()
{
  m_pRunningThread =
    std::make_shared<std::thread>([m_ctx = m_ctx]() { m_ctx->run(); });
}
