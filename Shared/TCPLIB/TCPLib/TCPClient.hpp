#ifndef TCP_CLIENT_HPP
#define TCP_CLIENT_HPP

#include <condition_variable>
#include <map>
#include <mutex>
#include <mutex>
#include <thread>

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/thread_pool.hpp>
#include <openssl/rsa.h>

#include "ACQLib/ACQ.hpp"
#include "Handler.hpp"
#include "LoggerLib/Logger.hpp"
#include "MsgLib/BaseMsg.hpp"
#include "RegistryLib/Registry.hpp"
#include "TCPTools.hpp"

namespace tcp
{
  class TcpClient
  {
  public:
    TcpClient(std::string hostname,
              std::string port,
              msg::FORMAT format = msg::FORMAT::PROTOBUF,
              std::string name = "",
              bool encrypt = false);

    ~TcpClient();

    template <class T>
    boost::signals2::scoped_connection registerHandler(
      std::function<void(T, std::string)> handler)
    {
      auto poster = [this](T msg,
                           std::function<void(T, std::string)> f,
                           const std::string& msgId) {
        GlobalRegistry::getRegistry().postToThreadPool(
          [msg, f, msgId]() { f(msg, msgId); });
      };
      boost::signals2::slot<void(T, std::string)> slot = [poster, handler](
        T msg, const std::string& msgId) { poster(msg, handler, msgId); };
      auto pHandle = std::make_shared<Handler<T>>();
      m_handlers->add(T::name(), pHandle);
      return pHandle->signal().connect(slot);
    }

    template <class T>
    void send(T message)
    {
      if (!m_ready)
      {
        std::unique_lock<std::mutex> lock(m_m);
        m_cv.wait(lock);
      }
      static int c = 0;
      msg::BaseMsg msg;
      msg.msg(msg::toString(message, m_format));
      msg.type(T::name());
      msg.msgId(m_name + std::to_string(c++));
      std::string toSend(msg::toString(msg, m_format));

      if (m_encrypted)
      {
        char* decrypted = new char[1024];
        int ret = RSA_public_encrypt(static_cast<int>(toSend.size()),
                                     (unsigned char*)toSend.c_str(),
                                     (unsigned char*)decrypted,
                                     m_pKey.get(),
                                     RSA_PKCS1_PADDING);
        if (ret < 0)
        {
          logger::logError("TCPClient", "Failed to encrypt");
          return;
        }

        toSend = std::string(decrypted, ret);
        delete[] decrypted;
      }
      auto pMessage =
        std::make_shared<std::string>(tcp::getProcessedString(toSend));
      m_socket.async_write_some(
        boost::asio::buffer(*pMessage, pMessage.get()->size()),
        [this, pMessage](auto a, auto b) { this->handleWrite(a, b); });
      (*m_pMessages)[msg.msgId()] = {
        msg, std::chrono::steady_clock::now() + std::chrono::seconds(10)};
    }

    template <class T>
    void respond(T message, const std::string& msgId)
    {
      msg::BaseMsg msg;
      msg.msg(msg::toString(message, m_format));
      msg.type(T::name());
      msg.msgId(msgId);
      std::string toSend(msg::toString(msg, m_format));

      if (m_encrypted)
      {
        char* decrypted = new char[1024];
        int ret = RSA_public_encrypt(static_cast<int>(toSend.size()),
                                     (unsigned char*)toSend.c_str(),
                                     (unsigned char*)decrypted,
                                     m_pKey.get(),
                                     RSA_PKCS1_PADDING);
        if (ret < 0)
        {
          logger::logError("TCPClient", "Failed to encrypt");
          return;
        }

        toSend = std::string(decrypted, ret);
        delete[] decrypted;
      }

      auto pMessage =
        std::make_shared<std::string>(tcp::getProcessedString(toSend));
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

    bool isConnected();

    void checkMsgs();

    void resend(const msg::BaseMsg& msg);

  private:
    std::condition_variable m_cv;
    std::mutex m_m;
    bool m_encrypted;
    bool m_ready;
    std::shared_ptr<RSA> m_pKey;
    boost::asio::io_context m_ctx;
    boost::optional<boost::asio::io_context::work> m_optCork;
    msg::FORMAT m_format;
    std::string m_name;
    boost::asio::steady_timer m_timer;
    std::shared_ptr<std::map<std::string, msg::ResendMsg>> m_pMessages;
    std::thread m_ctxThread;
    boost::asio::ip::tcp::socket m_socket;
    std::vector<char> m_inputBuffer;
    std::shared_ptr<tcp::HandlerMap> m_handlers;
    AutoConsumedQueue m_acq;
  };
} // namespace tcp

#endif
