#ifndef TCP_SERVER_HPP
#define TCP_SERVER_HPP

#include <string>
#include <iostream>
#include <thread>

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/thread_pool.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/optional.hpp>
#include <boost/signals2.hpp>

#include "ACQLib/ACQ.hpp"

namespace tcp
{
  class TcpServer
  {
  private:
    class TcpConnection : public std::enable_shared_from_this<TcpConnection>
    {
    public:
      template <class T>
      static std::shared_ptr<TcpConnection> create(
        boost::asio::io_context& ctx,
        int id,
        boost::asio::thread_pool& pool,
        std::function<T(std::string)> parser,
        std::map<std::string, std::function<void()>>& handlers)
      {
        return std::shared_ptr<TcpServer::TcpConnection>(
          new TcpServer::TcpConnection(ctx, id, pool, parser, handlers));
      }

      boost::asio::ip::tcp::socket& socket();

      void send(std::string message);

      int getId() { return m_id; }

      void registerDisconnect(
        boost::signals2::signal<void(int)>::slot_type slot);

      void startRead();

      void handleRead(const boost::system::error_code& ec,
                      std::size_t bytes_transferred);

    private:
      template <class T>
      TcpConnection(
        boost::asio::io_context& ctx,
        int id,
        boost::asio::thread_pool& pool,
        std::function<T(std::string)> parser,
        std::map<std::string, std::function<void()>>& handlers)
        : m_socket(ctx),
        m_id(id),
        m_closedSignal(),
        m_threadPool(pool),
        m_parser(parser),
        m_inputBuffer(1024),
        m_handlers(handlers),
        m_acq([m_threadPool = &m_threadPool,
          m_parser = &m_parser,
          m_handlers = &m_handlers](std::string& input) {
        static std::string inputBuffer;
        auto msg = tcp::getNextStringMessage(inputBuffer, input);
        if (!msg) return;
        boost::asio::post(*m_threadPool, [msg, m_parser, m_handlers]() {
          T pMsg = (*m_parser)(msg.get());
          m_handlers->find(pMsg->getName())->second();
        });
      })
      {
      }

      void handleWrite(const boost::system::error_code& error, size_t bt);

      boost::asio::ip::tcp::socket m_socket;
      int m_id;
      boost::signals2::signal<void(int)> m_closedSignal;
      boost::asio::thread_pool& m_threadPool;
      std::map<std::string, std::function<void()>>& m_handlers;
      template <class T>
      std::function<T(std::string)> m_parser;
      std::vector<char> m_inputBuffer;
      AutoConsumedQueue m_acq;
    };

  public:
    template <class T>
    TcpServer(unsigned short port,
              boost::asio::thread_pool& pool,
              std::function<T(std::string)> parser)
      : m_ctx(),
      m_optCork(m_ctx),
      m_iocThread([m_ctx = &m_ctx]() { m_ctx->run(); }),
      m_pAcceptor(
        m_ctx, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
      m_connections(),
      m_threadPool(pool),
      m_parser(parser)
    {
      startAccept();
    }

    ~TcpServer();

    void startAccept();

    void handleAccept(std::shared_ptr<TcpConnection> newConnection,
                      const boost::system::error_code& error);

    template <class T>
    boost::signals2::connection registerHandler(
      const std::string& key,
      function<void(T)> handler)
    {
      auto poster = [&m_threadPool](T msg, function<void(T)> f) { m_threadPool.post([msg, f]() {f(msg); }; };
      m_handlers[key].connect([poster, handler]() {poster(); });
    }

    void sendToAll(const std::string& message);

  private:
    boost::asio::io_context m_ctx;
    boost::optional<boost::asio::io_context::work> m_optCork;
    std::thread m_iocThread;
    boost::asio::ip::tcp::acceptor m_pAcceptor;
    std::map<int, std::shared_ptr<TcpConnection>> m_connections;
    boost::asio::thread_pool& m_threadPool;
    template <class T>
    std::function<T(std::string)> m_parser;
    std::map<std::string, boost::signals2::signal<void()>> m_handlers;
  };
} // namespace tcp

#endif