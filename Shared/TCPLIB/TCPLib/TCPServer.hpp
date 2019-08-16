#ifndef TCP_SERVER_HPP
#define TCP_SERVER_HPP

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
#include "TCPTools.hpp"

#include "ACQLib/ACQ.hpp"

namespace tcp
{
  class TcpServer
  {
  public:
    class TcpConnection : public std::enable_shared_from_this<TcpConnection>
    {
    public:
      static std::shared_ptr<TcpConnection> create(
        boost::asio::io_context& ctx, int id, boost::asio::thread_pool& pool);

      boost::asio::ip::tcp::socket& socket();

      template <class T>
      void send(T message, const msg::FORMAT& format)
      {
        msg::BaseMsg msg;
        msg.msg(msg::toString(message, format));
        msg.type(T::name());
        auto pMessage = std::make_shared<std::string>(
          tcp::getProcessedString(msg::toString(msg, format)));
        m_socket.async_write_some(
          boost::asio::buffer(*pMessage, pMessage.get()->size()),
          [this, pMessage](auto a, auto b) { this->handleWrite(a, b); });
      }

      int getId() { return m_id; }

      void registerDisconnect(
        boost::signals2::signal<void(int)>::slot_type slot);

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
          boost::asio::post(m_threadPool, [msg, f]() { f(msg); });
        };
        boost::signals2::slot<void(T)> slot = [poster, handler](T msg) {
          poster(msg, handler);
        };
        auto pHandle = std::make_shared<Handler<T>>();
        m_handlers->add(T::name(), pHandle);
        return pHandle->signal().connect(slot);
      }

    private:
      TcpConnection(boost::asio::io_context& ctx,
                    int id,
                    boost::asio::thread_pool& pool);

      void handleWrite(const boost::system::error_code& error, size_t bt);

      boost::asio::ip::tcp::socket m_socket;
      int m_id;
      boost::signals2::signal<void(int)> m_closedSignal;
      boost::asio::thread_pool& m_threadPool;
      std::vector<char> m_inputBuffer;
      std::shared_ptr<tcp::HandlerMap> m_handlers;
      AutoConsumedQueue m_acq;
    };

  public:
    TcpServer(unsigned short port, boost::asio::thread_pool& pool);

    ~TcpServer();

    void startAccept();

    void handleAccept(std::shared_ptr<TcpConnection> newConnection,
                      const boost::system::error_code& error);

    boost::signals2::scoped_connection registerConnection(
      std::function<void(std::shared_ptr<TcpConnection>)> handler)
    {
      return m_connectionHandler.connect(handler);
    }

    void close();

    template <class T>
    void sendToAll(const T& message)
    {
      for (auto&& connection : m_connections)
      {
        connection.second->send(message);
      }
    }

  private:
    boost::asio::io_context m_ctx;
    boost::optional<boost::asio::io_context::work> m_optCork;
    std::thread m_iocThread;
    boost::asio::ip::tcp::acceptor m_pAcceptor;
    std::map<int, std::shared_ptr<TcpConnection>> m_connections;
    boost::signals2::signal<void(std::shared_ptr<TcpConnection>)> m_connectionHandler;
    boost::asio::thread_pool& m_threadPool;
  };
} // namespace tcp

#endif
