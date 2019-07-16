#ifndef TCP_CLIENT_HPP
#define TCP_CLIENT_HPP

#include <map>

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/thread_pool.hpp>

#include "ACQLib/ACQ.hpp"
#include "MsgLib/BaseMsg.hpp"

namespace tcp
{
  class TcpClient
  {
  public:
    TcpClient(std::string hostname,
              std::string port,
              boost::asio::thread_pool& pool,
              std::function<msg::BaseMsg(std::string)> parser);

    ~TcpClient();

    void registerHandler(
      const std::string& key,
      std::function<void(std::shared_ptr<msg::BaseMsg>)> handler);

    void send(const std::string& message);

    void startConnect(boost::asio::ip::tcp::resolver::iterator endpoint_iter);

    void handleConnect(const boost::system::error_code& ec,
                       boost::asio::ip::tcp::resolver::iterator endpoint_iter);

    void startRead();

    void handleRead(const boost::system::error_code& ec, std::size_t bt);

  private:
    boost::asio::io_context m_ctx;
    boost::optional<boost::asio::io_context::work> m_optCork;
    std::thread m_ctxThread;
    boost::asio::thread_pool& m_threadPool;
    boost::asio::ip::tcp::socket m_socket;
    std::vector<char> m_inputBuffer;
    std::function<msg::BaseMsg(std::string)> m_parser;
    std::map<std::string, std::function<void(std::shared_ptr<msg::BaseMsg>)>>
      m_handlers;
    AutoConsumedQueue m_acq;
  };
} // namespace tcp

#endif