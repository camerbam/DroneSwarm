#ifndef TCP_CLIENT_HPP
#define TCP_CLIENT_HPP

#include <map>
#include <mutex>

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/thread_pool.hpp>

#include "ACQLib/ACQ.hpp"
#include "Handler.hpp"
#include "MsgLib/BaseMsg.hpp"
#include "MsgLib/SerializeDeserialize.hpp"
#include "TCPTools.hpp"

namespace tcp
{
  class TcpClient
  {
  public:
    TcpClient(std::string hostname,
              std::string port,
              std::shared_ptr<boost::asio::thread_pool> pool);

    ~TcpClient();

    template <class T>
    boost::signals2::scoped_connection registerHandler(
      std::function<void(T)> handler)
    {
      auto poster = [this](T msg, std::function<void(T)> f) {
        boost::asio::post(*m_pThreadPool, [msg, f]() { f(msg); });
      };
      boost::signals2::slot<void(T)> slot = [poster, handler](T msg) {
        poster(msg, handler);
      };
      auto pHandle = std::make_shared<Handler<T>>();
      m_handlers->add(T::name(), pHandle);
      return pHandle->signal().connect(slot);
    }

    template <class T>
    void send(T message, const msg::FORMAT& format)
    {
      msg::BaseMsg msg;
      msg.msg(msg::toString(message, format));
      msg.type(T::name());
      auto pMessage = std::make_shared<std::string>(
        tcp::getProcessedString(toString(msg, format)));
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

    void ready();

  private:
    boost::asio::io_context m_ctx;
    boost::optional<boost::asio::io_context::work> m_optCork;
    std::thread m_ctxThread;
    std::shared_ptr<boost::asio::thread_pool> m_pThreadPool;
    boost::asio::ip::tcp::socket m_socket;
    std::vector<char> m_inputBuffer;
    std::shared_ptr<tcp::HandlerMap> m_handlers;
    AutoConsumedQueue m_acq;
  };
} // namespace tcp

#endif
