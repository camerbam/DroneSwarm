#ifndef TCP_CONNECTION_HPP
#define TCP_CONNECTION_HPP

#include <iostream>
#include <string>
#include <thread>

#include <boost/asio/buffer.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/thread_pool.hpp>
#include <boost/optional.hpp>
#include <boost/signals2.hpp>

#include "Handler.hpp"
#include "MsgLib/BaseMsg.hpp"
#include "MsgLib/MsgTypes.hpp"
#include "TCPTools.hpp"

#include "ACQLib/ACQ.hpp"

namespace tcp
{
  class TcpConnection : public std::enable_shared_from_this<TcpConnection>
  {
  public:
    static std::shared_ptr<TcpConnection> create(
      std::shared_ptr<boost::asio::ip::tcp::socket> pSocket,
      int id,
      msg::FORMAT format);

    template <class T>
    void send(T message)
    {
      msg::BaseMsg msg;
      msg.msg(msg::toString(message, m_format));
      msg.type(T::name());
      auto pMessage = std::make_shared<std::string>(
        tcp::getProcessedString(msg::toString(msg, m_format)));
      m_pSocket->async_write_some(
        boost::asio::buffer(*pMessage, pMessage.get()->size()),
        [this, pMessage](auto a, auto b) { this->handleWrite(a, b); });
    }

    int getId() { return m_id; }

    void registerDisconnect(boost::signals2::signal<void(int)>::slot_type slot);

    void startRead();

    void ready();

    void close();

    void handleRead(const boost::system::error_code& ec,
                    std::size_t bytes_transferred);

    template <class T>
    boost::signals2::scoped_connection registerHandler(
      std::function<void(T)> handler)
    {
      auto poster = [this](T msg, std::function<void(T)> f) {
        boost::asio::post(GlobalRegistry::getRegistry().getThreadPool(),
                          [msg, f]() { f(msg); });
      };
      boost::signals2::slot<void(T)> slot = [poster, handler](T msg) {
        poster(msg, handler);
      };
      auto pHandle = std::make_shared<Handler<T>>();
      m_handlers->add(T::name(), pHandle);
      return pHandle->signal().connect(slot);
    }

  private:
    TcpConnection(std::shared_ptr<boost::asio::ip::tcp::socket> pSocket,
                  int id,
                  msg::FORMAT format);

    void handleWrite(const boost::system::error_code& error, size_t bt);

    std::shared_ptr<boost::asio::ip::tcp::socket> m_pSocket;
    int m_id;
    msg::FORMAT m_format;
    boost::signals2::signal<void(int)> m_closedSignal;
    std::vector<char> m_inputBuffer;
    std::shared_ptr<tcp::HandlerMap> m_handlers;
    AutoConsumedQueue m_acq;
  };
} // namespace tcp

#endif
