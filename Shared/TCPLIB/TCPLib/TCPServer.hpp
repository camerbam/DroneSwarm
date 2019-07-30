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

#include "BaseMsg.hpp"
#include "Handler.hpp"
#include "TCPTools.hpp"

#include "ACQLib/ACQ.hpp"

namespace tcp
{
  class TcpServer
  {
  private:
    class TcpConnection : public std::enable_shared_from_this<TcpConnection>
    {
    public:
      static std::shared_ptr<TcpConnection> create(
        boost::asio::io_context& ctx,
        int id,
        boost::asio::thread_pool& pool,
        tcp::HandlerMap& handlers)
      {
        return std::shared_ptr<TcpServer::TcpConnection>(
          new TcpServer::TcpConnection(ctx, id, pool, handlers));
      }

      boost::asio::ip::tcp::socket& socket();

      template <class T>
      void send(T message, const FORMAT& format)
      {
        tcp::BaseMsg msg;
        msg.format(format);
        msg.msg(message.toString());
        msg.type(T::name());
        auto pMessage = std::make_shared<std::string>(
          tcp::getProcessedString(msg.toString()));
        m_socket.async_write_some(
          boost::asio::buffer(*pMessage, pMessage.get()->size()),
          [this, pMessage](auto a, auto b) { this->handleWrite(a, b); });
      }

      int getId() { return m_id; }

      void registerDisconnect(
        boost::signals2::signal<void(int)>::slot_type slot);

      void startRead();

      void handleRead(const boost::system::error_code& ec,
                      std::size_t bytes_transferred);

    private:
      TcpConnection(boost::asio::io_context& ctx,
                    int id,
                    boost::asio::thread_pool& pool,
                    tcp::HandlerMap& handlers)
        : m_socket(ctx),
          m_id(id),
          m_closedSignal(),
          m_threadPool(pool),
          m_inputBuffer(1024),
          m_handlers(handlers),
          m_acq([m_threadPool = &m_threadPool, m_handlers = &m_handlers](
                  std::string& input, std::mutex& mutex) {
            std::lock_guard<std::mutex> lock(mutex);
            auto msg = tcp::getNextStringMessage(input);
            if (!msg) return;
            boost::asio::post(*m_threadPool, [msg, &m_handlers]() {
              tcp::BaseMsg receivedMsg;
              // TODO: Check return bool
              receivedMsg.parseString(msg.get());
              auto handle = m_handlers->get(receivedMsg.type());
              if (!handle) std::cout << "Received unknown message" << std::endl;
              handle->execute(receivedMsg.msg());
            });
          })
      {
      }

      void handleWrite(const boost::system::error_code& error, size_t bt);

      boost::asio::ip::tcp::socket m_socket;
      int m_id;
      boost::signals2::signal<void(int)> m_closedSignal;
      boost::asio::thread_pool& m_threadPool;
      tcp::HandlerMap& m_handlers;
      std::vector<char> m_inputBuffer;
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
        m_threadPool(pool),
        m_handlers()
    {
      startAccept();
    }

    ~TcpServer();

    void startAccept();

    void handleAccept(std::shared_ptr<TcpConnection> newConnection,
                      const boost::system::error_code& error);

    template <class T>
    boost::signals2::scoped_connection registerHandler(
      std::function<void(T)> handler)
    {
      // TODO I don't like capturing this
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
    void sendToAll(const T& message, const FORMAT& format)
    {
      for (auto&& connection : m_connections)
      {
        connection.second->send(message, format);
      }
    }

  private:
    boost::asio::io_context m_ctx;
    boost::optional<boost::asio::io_context::work> m_optCork;
    std::thread m_iocThread;
    boost::asio::ip::tcp::acceptor m_pAcceptor;
    std::map<int, std::shared_ptr<TcpConnection>> m_connections;
    boost::asio::thread_pool& m_threadPool;
    tcp::HandlerMap m_handlers;
  };
} // namespace tcp

#endif