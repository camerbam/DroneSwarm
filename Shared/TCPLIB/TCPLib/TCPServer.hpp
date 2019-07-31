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
        boost::asio::io_context& ctx, int id, boost::asio::thread_pool& pool)
      {
        return std::shared_ptr<TcpServer::TcpConnection>(
          new TcpServer::TcpConnection(ctx, id, pool));
      }

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
        m_handlers.add(T::name(), pHandle);
        return pHandle->signal().connect(slot);
      }

    private:
      TcpConnection(boost::asio::io_context& ctx,
                    int id,
                    boost::asio::thread_pool& pool)
        : m_socket(ctx),
          m_id(id),
          m_closedSignal(),
          m_threadPool(pool),
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
                if(!msg::parseString(receivedMsg, msg, format))
                {
                  std::cout << "Could not parse msg" << std::endl;
                  return;
                }
                auto handle = m_handlers->get(receivedMsg.type());
                if (!handle)
                  std::cout << "Received unknown message" << std::endl;
                handle->execute(receivedMsg.msg(), format);
              });
            }
          })
      {
      }

      void handleWrite(const boost::system::error_code& error, size_t bt);

      boost::asio::ip::tcp::socket m_socket;
      int m_id;
      boost::signals2::signal<void(int)> m_closedSignal;
      boost::asio::thread_pool& m_threadPool;
      std::vector<char> m_inputBuffer;
      tcp::HandlerMap m_handlers;
      AutoConsumedQueue m_acq;
    };

  public:
    TcpServer(unsigned short port, boost::asio::thread_pool& pool)
      : m_ctx(),
        m_optCork(m_ctx),
        m_iocThread([m_ctx = &m_ctx]() { m_ctx->run(); }),
        m_pAcceptor(
          m_ctx,
          boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
        m_connections(),
        m_threadPool(pool)
    {
      startAccept();
    }

    ~TcpServer();

    void startAccept();

    void handleAccept(std::shared_ptr<TcpConnection> newConnection,
                      const boost::system::error_code& error);

    void registerConnection(
      std::function<void(std::shared_ptr<TcpConnection>)> handler)
    {
      m_connectionHandler = handler;
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
    std::function<void(std::shared_ptr<TcpConnection>)> m_connectionHandler;
    boost::asio::thread_pool& m_threadPool;
  };
} // namespace tcp

#endif
