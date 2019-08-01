#include "ACQ.hpp"

AutoConsumedQueue::AutoConsumedQueue(std::function<void(std::string)> handler)
  : m_running(false),
  m_handler(handler),
  m_ctx(std::make_shared<boost::asio::io_context>()),
  m_optCork(*m_ctx),
  m_runningThread([m_ctx = m_ctx]() { m_ctx->run(); })
{
}

AutoConsumedQueue::~AutoConsumedQueue()
{
  m_optCork = boost::none;
  m_runningThread.join();
}

void AutoConsumedQueue::add(const std::string& input)
{
  m_running = true;
  m_ctx->post([m_handler = m_handler, m_running = &m_running, input] {
    m_handler(input);
    *m_running = false;
  });
}
