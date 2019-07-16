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
#include "MsgLib/BaseMsg.hpp"

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
        std::function<msg::BaseMsg(std::string)> parser,
        std::map<std::string, std::function<void(std::shared_ptr<msg::BaseMsg>)>>& handlers);

      boost::asio::ip::tcp::socket& socket();

      void send(std::string message);

      int getId() { return m_id; }

      void registerDisconnect(
        boost::signals2::signal<void(int)>::slot_type slot);

      void startRead();

      void handleRead(const boost::system::error_code& ec,
                      std::size_t bytes_transferred);

    private:
      TcpConnection(
        boost::asio::io_context& ctx,
        int id,
        boost::asio::thread_pool& pool,
        std::function<msg::BaseMsg(std::string)> parser,
        std::map<std::string, std::function<void(std::shared_ptr<msg::BaseMsg>)>>& handlers);

      void handleWrite(const boost::system::error_code& error, size_t bt);

      boost::asio::ip::tcp::socket m_socket;
      int m_id;
      boost::signals2::signal<void(int)> m_closedSignal;
      boost::asio::thread_pool& m_threadPool;
      std::map<std::string, std::function<void(std::shared_ptr<msg::BaseMsg>)>>& m_handlers;
      std::function<msg::BaseMsg(std::string)> m_parser;
      std::vector<char> m_inputBuffer;
      AutoConsumedQueue m_acq;
    };

  public:
    TcpServer(unsigned short port,
              boost::asio::thread_pool& pool,
              std::function<msg::BaseMsg(std::string)> parser);

    ~TcpServer();

    void startAccept();

    void handleAccept(std::shared_ptr<TcpConnection> newConnection,
                      const boost::system::error_code& error);

    void registerHandler(
      const std::string& key,
      std::function<void(std::shared_ptr<msg::BaseMsg>)> handler);

    void sendToAll(const std::string& message);

  private:
    boost::asio::io_context m_ctx;
    boost::optional<boost::asio::io_context::work> m_optCork;
    std::thread m_iocThread;
    boost::asio::ip::tcp::acceptor m_pAcceptor;
    std::map<int, std::shared_ptr<TcpConnection>> m_connections;
    boost::asio::thread_pool& m_threadPool;
    std::function<msg::BaseMsg(std::string)> m_parser;
    std::map<std::string, std::function<void(std::shared_ptr<msg::BaseMsg>)>>
      m_handlers;
  };
} // namespace tcp

#endif