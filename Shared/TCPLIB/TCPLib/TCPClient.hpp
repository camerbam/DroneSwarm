#ifndef TCP_CLIENT_HPP
#define TCP_CLIENT_HPP

#include <map>
#include <mutex>

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/thread_pool.hpp>

// boost expiremental
#include "TCPTools.hpp"
#include <boost/asio/buffer.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/post.hpp>
#include <boost/asio/thread_pool.hpp>
#include <boost/optional.hpp>
#include <boost/signals2.hpp>

#include "ACQLib/ACQ.hpp"
#include "Handler.hpp"
#include "MsgLib/BaseMsg.hpp"
#include "MsgLib/SerializeDeserialize.hpp"

namespace tcp
{
  class TcpClient
  {
  public:
    TcpClient(std::string hostname,
              std::string port,
              boost::asio::thread_pool& pool)
      : m_ctx(),
        m_optCork(m_ctx),
        m_ctxThread([m_ctx = &m_ctx]() { m_ctx->run(); }),
        m_threadPool(pool),
        m_socket(m_ctx),
        m_inputBuffer(1024),
        m_handlers(),
        m_acq([m_threadPool = &m_threadPool, m_handlers = &m_handlers](
                std::string& input, std::mutex& mutex) {
          std::lock_guard<std::mutex> lock(mutex);
          while (true)
          {
            auto optMsg = tcp::getNextStringMessage(input);
            if (!optMsg) return;
            boost::asio::post(*m_threadPool, [optMsg, &m_handlers]() {
              msg::BaseMsg receivedMsg;
              auto msg = optMsg.get();
              auto format = msg::getMsgFormat(msg);
              parseString(receivedMsg, msg, format);
              auto handle = m_handlers->get(receivedMsg.type());
              if (!handle)
              {
                std::cout << "Received unknown message" << std::endl;
                return;
              }
              handle->execute(receivedMsg.msg(), format);
            });
          }
        })
    {
      boost::asio::ip::tcp::resolver r(m_ctx);

      startConnect(r.resolve(boost::asio::ip::tcp::resolver::query(
        boost::asio::ip::tcp::v4(), hostname, port)));
    }

    ~TcpClient();

    template <class T>
    boost::signals2::scoped_connection registerHandler(
      std::function<void(T)> handler)
    {
      auto poster = [this](T msg, std::function<void(T)> f) {
        boost::asio::post(m_threadPool, [msg, f]() { f(msg); });
      };
      boost::signals2::slot<void(T)> slot = [poster, handler](T msg) {
        poster(msg, handler);
      };
      auto pHandle = std::make_shared<Handler<T>>();
      m_handlers.add(T::name(), pHandle);
      return pHandle->signal().connect(slot);
    }

    template <class T>
    void send(T message, const msg::FORMAT& format)
    {
      msg::BaseMsg msg;
      msg.msg(msg::toString(message, format));
      msg.type(T::name());
      auto pMessage =
        std::make_shared<std::string>(tcp::getProcessedString(toString(msg, format)));
      m_socket.async_write_some(
        boost::asio::buffer(*pMessage, pMessage.get()->size()),
        [this, pMessage](auto a, auto b) { this->handleWrite(a, b); });
    }

    void handleWrite(const boost::system::error_code& error, size_t bt);

    void startConnect(boost::asio::ip::tcp::resolver::iterator endpoint_iter);

    void handleConnect(const boost::system::error_code& ec,
                       boost::asio::ip::tcp::resolver::iterator endpoint_iter);

    void startRead();

    void handleRead(const boost::system::error_code& ec, std::size_t bt);

    void close();

  private:
    boost::asio::io_context m_ctx;
    boost::optional<boost::asio::io_context::work> m_optCork;
    std::thread m_ctxThread;
    boost::asio::thread_pool& m_threadPool;
    boost::asio::ip::tcp::socket m_socket;
    std::vector<char> m_inputBuffer;
    tcp::HandlerMap m_handlers;
    AutoConsumedQueue m_acq;
  };
} // namespace tcp

#endif