#ifndef TCP_SERVER_HPP
#define TCP_SERVER_HPP

#include <condition_variable>
#include <iostream>
#include <string>
#include <thread>

#include <boost/asio/buffer.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/optional.hpp>
#include <boost/signals2.hpp>

#include "Handler.hpp"
#include "MsgLib/BaseMsg.hpp"
#include "TCPConnection.hpp"
#include "TCPTools.hpp"

#include "ACQLib/ACQ.hpp"

namespace tcp
{
  class TcpServer
  {
  public:
    TcpServer(unsigned short port,
              msg::FORMAT format = msg::FORMAT::PROTOBUF,
              bool encrypted = false);

    ~TcpServer();

    void startAccept();

    void handleAccept(std::shared_ptr<boost::asio::ip::tcp::socket> pSocket,
                      const boost::system::error_code& error);

    boost::signals2::scoped_connection registerConnection(
      std::function<void(std::shared_ptr<TcpConnection>)> handler)
    {
      return m_connectionHandler.connect(handler);
    }

    template <class T>
    void sendToAll(const T& message)
    {
      for (auto&& connection : m_connections)
      {
        connection.second->send(message);
      }
    }

    void close();

    void checkMsgs();

  private:
    bool m_encrypted;
    std::shared_ptr<RSA> m_privateKey;
    std::string m_publicKey;
    std::shared_ptr<boost::asio::io_context> m_pCtx;
    boost::optional<boost::asio::io_context::work> m_optCork;
    msg::FORMAT m_format;
    boost::asio::steady_timer m_timer;
    boost::asio::ip::tcp::acceptor m_pAcceptor;
    std::map<int, std::shared_ptr<TcpConnection>> m_connections;
    boost::signals2::signal<void(std::shared_ptr<TcpConnection>)>
      m_connectionHandler;
    std::thread m_iocThread;
  };
} // namespace tcp

#endif
