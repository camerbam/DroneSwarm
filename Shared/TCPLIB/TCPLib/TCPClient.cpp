#include "TCPClient.hpp"

#include <iostream>

#include <boost/asio/write.hpp>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>

#include "EncryptHelpers.hpp"
#include "LoggerLib/Logger.hpp"
#include "RegistryLib/Registry.hpp"
#include "TCPTools.hpp"

tcp::TcpClient::TcpClient(std::string hostname,
                          std::string port,
                          msg::FORMAT format,
                          std::string name,
                          bool encrypt)
  : m_cv(),
    m_m(),
    m_encrypted(encrypt),
    m_ready(false),
    m_pKey(std::make_shared<RSA>()),
    m_ctx(),
    m_optCork(m_ctx),
    m_format(format),
    m_name(name),
    m_timer(m_ctx),
    m_pMessages(std::make_shared<std::map<std::string, msg::ResendMsg>>()),
    m_ctxThread([m_ctx = &m_ctx]() { m_ctx->run(); }),
    m_socket(m_ctx),
    m_inputBuffer(1024),
    m_handlers(std::make_shared<tcp::HandlerMap>()),
    m_acq(std::function<void(std::string)>([
      m_handlers = m_handlers,
      format = m_format,
      m_pMessages = m_pMessages,
      m_encrypted = m_encrypted,
      m_pKey = &m_pKey
    ](std::string input) {
      while (true)
      {
        auto optMsg = tcp::getNextStringMessage(input);
        if (!optMsg) return;
        GlobalRegistry::getRegistry().postToThreadPool([
          optMsg,
          m_handlers,
          format,
          m_pMessages,
          m_encrypted,
          m_pKey
        ]() {
          msg::BaseMsg receivedMsg;
          auto msg = optMsg.get();
          if (m_encrypted)
          {
            char* decrypted = new char[1024];
            int result = RSA_public_decrypt(static_cast<int>(msg.size()),
                                            (unsigned char*)msg.c_str(),
                                            (unsigned char*)decrypted,
                                            m_pKey->get(),
                                            RSA_PKCS1_PADDING);
            if (result < 0)
            {
              char buffer[120];
              ERR_error_string(ERR_get_error(), buffer);
              printf("Error reading private key:%s\n", buffer);
              logger::logError("TCPClient", "Failed to decypt message");
              return;
            }
            msg = std::string(decrypted, result);
            delete[] decrypted;
          }
          if (!parseString(receivedMsg, msg, format))
          {
            logger::logError("TCPClient", "Could not parse msg");
            return;
          }

          auto msgSent = m_pMessages->find(receivedMsg.msgId());
          auto handle = m_handlers->get(receivedMsg.type());
          if (handle)
            handle->execute(receivedMsg.msg(), format, receivedMsg.msgId());
          if (msgSent != m_pMessages->end()) m_pMessages->erase(msgSent);
          if (!handle)
            std::cout << "Received unknown message: " << receivedMsg.type()
                      << std::endl;
        });
      }
    }))
{
  boost::asio::ip::tcp::resolver r(m_ctx);

  startConnect(r.resolve(boost::asio::ip::tcp::resolver::query(
    boost::asio::ip::tcp::v4(), hostname, port)));

  m_timer.expires_after(boost::asio::chrono::seconds(1));
  m_timer.async_wait([this](const boost::system::error_code& e) {
    if (!e) checkMsgs();
  });
}

tcp::TcpClient::~TcpClient()
{
  m_optCork = boost::none;
  if (m_ctxThread.joinable()) m_ctxThread.join();
}

void tcp::TcpClient::startConnect(
  boost::asio::ip::tcp::resolver::iterator endpoint_iter)
{
  if (endpoint_iter != boost::asio::ip::tcp::resolver::iterator())
  {
    std::cout << "Trying " << endpoint_iter->endpoint() << "...\n";

    // Start the asynchronous connect operation.
    m_socket.async_connect(
      endpoint_iter->endpoint(),
      [this, endpoint_iter](const boost::system::error_code& ec) {
        handleConnect(ec, endpoint_iter);
      });
  }
  else
  {
    std::cout << "No more endpoints to try" << std::endl;
    close();
  }
}

void tcp::TcpClient::handleWrite(const boost::system::error_code&, size_t)
{
  // if (e)
  //{
  //  std::cout << e.message() << std::endl;
  //}
  // std::cout << a << std::endl;
}

void tcp::TcpClient::handleConnect(
  const boost::system::error_code& ec,
  boost::asio::ip::tcp::resolver::results_type::iterator endpoint_iter)
{
  // The async_connect() function automatically opens the socket at the start
  // of the asynchronous operation. If the socket is closed at this time then
  // the timeout handler must have run first.
  if (!m_socket.is_open())
  {
    std::cout << "Connect timed out\n";

    // Try the next available endpoint.
    startConnect(++endpoint_iter);
  }

  // Check if the connect operation failed before the deadline expired.
  else if (ec)
  {
    std::cout << "Connect error: " << ec.message() << "\n";

    // We need to close the socket used in the previous connection attempt
    // before starting a new one.
    m_socket.close();

    // Try the next available endpoint.
    startConnect(++endpoint_iter);
  }

  // Otherwise we have successfully established a connection.
  else
  {
    std::cout << "Connected to " << endpoint_iter->endpoint() << "\n";

    if (m_encrypted)
    {
      auto read = m_socket.read_some(boost::asio::buffer(m_inputBuffer));
      std::string toAdd(m_inputBuffer.begin(), m_inputBuffer.begin() + read);
      RSA* rsa = tcp::createPublicRSA(toAdd);
      m_pKey.reset(rsa);
    }
    m_ready = true;
    m_cv.notify_all();

    // Start the input actor.
    startRead();
  }
}

void tcp::TcpClient::startRead()
{
  m_socket.async_receive(boost::asio::buffer(m_inputBuffer),
                         [this](const boost::system::error_code& ec,
                                std::size_t bt) { handleRead(ec, bt); });
}

void tcp::TcpClient::handleRead(const boost::system::error_code& ec,
                                std::size_t bt)
{
  if (!ec)
  {
    std::string toAdd(m_inputBuffer.begin(), m_inputBuffer.begin() + bt);
    m_acq.add(toAdd);
    m_inputBuffer.clear();
    m_inputBuffer.resize(1024);

    startRead();
  }
  else
  {
    std::cout << "Error on receive: " << ec.message() << "\n";

    close();
  }
}

void tcp::TcpClient::close()
{
  m_timer.cancel();
  m_optCork = boost::none;
  m_ctx.stop();
}

void tcp::TcpClient::ready()
{
  m_acq.ready();
}

bool tcp::TcpClient::isConnected()
{
  return m_socket.is_open();
}

void tcp::TcpClient::resend(const msg::BaseMsg& msg)
{
  auto pMessage = std::make_shared<std::string>(
    tcp::getProcessedString(msg::toString(msg, m_format)));
  m_socket.async_write_some(
    boost::asio::buffer(*pMessage, pMessage.get()->size()),
    [this, pMessage](auto a, auto b) { this->handleWrite(a, b); });
  (*m_pMessages)[msg.msgId()].expireTime =
    std::chrono::steady_clock::now() + std::chrono::seconds(10);
  (*m_pMessages)[msg.msgId()].retries++;
}

void tcp::TcpClient::checkMsgs()
{
  m_timer.expires_after(boost::asio::chrono::seconds(1));
  m_timer.async_wait([this](const boost::system::error_code& e) {
    if (!e) checkMsgs();
  });
  auto now = std::chrono::steady_clock::now();
  for (auto m : *m_pMessages)
    if (m.second.expireTime < now) resend(m.second.msg);
}
