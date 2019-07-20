#include "ACQ.hpp"

#include <boost/asio/buffers_iterator.hpp>

AutoConsumedQueue::AutoConsumedQueue(std::function<void(std::string&, std::mutex&)> handler)
  : m_running(false),
    m_handler(handler),
    m_ctx(),
    m_optCork(m_ctx),
    m_bufferMutex(),
    m_buffer(),
    m_runningThread([m_ctx = &m_ctx]() { m_ctx->run(); })
{
}

AutoConsumedQueue::~AutoConsumedQueue()
{
  m_optCork = boost::none;
  m_runningThread.join();
}

void AutoConsumedQueue::addBytes(std::vector<char>& buf)
{
  {
    std::lock_guard<std::mutex> lock(m_bufferMutex);
    std::copy(buf.begin(), buf.end(), std::back_inserter(m_buffer));
  }

  if (!m_running)
  {
    m_running = true;
    m_ctx.post([this] {
      m_handler(m_buffer, m_bufferMutex);
      m_running = false;
    });
  }
}
