#include "ACQ.hpp"

#include <boost/asio/buffers_iterator.hpp>

AutoConsumedQueue::AutoConsumedQueue(std::function<void(std::string&)> handler)
  : m_running(false),
    m_handler(handler),
    m_ctx(),
    m_optCork(m_ctx),
    m_buffer(),
    m_runningThread([m_ctx = &m_ctx]() { m_ctx->run(); })
{
}

AutoConsumedQueue::~AutoConsumedQueue()
{
  m_optCork = boost::none;
  m_runningThread.join();
}

void AutoConsumedQueue::addBytes(boost::asio::streambuf& buf)
{
  m_buffer.append(boost::asio::buffers_begin(buf.data()),
                  boost::asio::buffers_end(buf.data()));
  if (!m_running)
  {
    m_running = true;
    m_ctx.post([this] {
      m_handler(m_buffer);
      m_running = false;
    });
  }
}
