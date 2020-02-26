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
#include <openssl/rsa.h>

#include "Handler.hpp"
#include "LoggerLib/Logger.hpp"
#include "MsgLib/BaseMsg.hpp"
#include "MsgLib/MsgTypes.hpp"
#include "RegistryLib/Registry.hpp"
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
      msg::FORMAT format,
      const std::string& privateKey);

    template <class T>
    void send(T message)
    {
      static int count = 0;
      msg::BaseMsg msg;
      std::string toSend(msg::toString(message, m_format));

      if (m_encrypted)
      {
        char* decrypted = new char[1024];
        int ret = RSA_private_encrypt(static_cast<int>(toSend.size()),
                                      (unsigned char*)toSend.c_str(),
                                      (unsigned char*)decrypted,
                                      m_pPrivateKey.get(),
                                      RSA_PKCS1_PADDING);
        if (ret < 0)
        {
          logger::logError("TCPConnection", "Failed to encrypt");
          return;
        }

        toSend = std::string(decrypted, ret);
        delete[] decrypted;
      }
      msg.msg(toSend);
      msg.type(T::name());
      msg.msgId(std::to_string(m_id) + ":" + std::to_string(count++));
      auto pMessage = std::make_shared<std::string>(
        tcp::getProcessedString(msg::toString(msg, m_format)));
      m_pSocket->async_write_some(
        boost::asio::buffer(*pMessage, pMessage.get()->size()),
        [this, pMessage](auto a, auto b) { this->handleWrite(a, b); });
      (*m_pMessages)[msg.msgId()] = {
        msg, std::chrono::steady_clock::now() + std::chrono::seconds(10)};
    }

    template <class T>
    void respond(T message, const std::string& msgId)
    {
      msg::BaseMsg msg;
      std::string toSend(msg::toString(message, m_format));

      if (m_encrypted)
      {
        char* decrypted = new char[1024];
        int ret = RSA_private_encrypt(static_cast<int>(toSend.size()),
          (unsigned char*)toSend.c_str(),
          (unsigned char*)decrypted,
          m_pPrivateKey.get(),
          RSA_PKCS1_PADDING);
        if (ret < 0)
        {
          logger::logError("TCPConnection", "Failed to encrypt");
          return;
        }

        toSend = std::string(decrypted, ret);
        delete[] decrypted;
      }
      msg.msg(toSend);
      msg.type(T::name());
      msg.msgId(msgId);
      auto pMessage = std::make_shared<std::string>(
        tcp::getProcessedString(msg::toString(msg, m_format)));
      m_pSocket->async_write_some(
        boost::asio::buffer(*pMessage, pMessage.get()->size()),
        [this, pMessage](auto a, auto b) { this->handleWrite(a, b); });
    }

    void resend(const msg::BaseMsg& msg);

    int getId() { return m_id; }

    void registerDisconnect(boost::signals2::signal<void(int)>::slot_type slot);

    void startRead();

    void ready();

    void close();

    void checkMsgs(const std::chrono::steady_clock::time_point& now);

    void handleRead(const boost::system::error_code& ec,
                    std::size_t bytes_transferred);

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

  private:
    TcpConnection(std::shared_ptr<boost::asio::ip::tcp::socket> pSocket,
                  int id,
                  msg::FORMAT format,
                  const std::string& privateKey);

    void handleWrite(const boost::system::error_code& error, size_t bt);

    bool m_encrypted;
    std::shared_ptr<RSA> m_pPrivateKey;
    std::shared_ptr<boost::asio::ip::tcp::socket> m_pSocket;
    int m_id;
    msg::FORMAT m_format;
    boost::signals2::signal<void(int)> m_closedSignal;
    std::vector<char> m_inputBuffer;
    std::shared_ptr<tcp::HandlerMap> m_handlers;
    std::shared_ptr<std::map<std::string, msg::ResendMsg>> m_pMessages;
    AutoConsumedQueue m_acq;
  };
} // namespace tcp

#endif
