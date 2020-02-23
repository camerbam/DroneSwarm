#include "TCPConnection.hpp"

#include <fstream>
#include <iostream>

#include <boost/asio.hpp>
#include <boost/asio/write.hpp>

#include "EncryptHelpers.hpp"
#include "LoggerLib/Logger.hpp"
#include "MsgLib/BaseMsg.hpp"
#include "RegistryLib/Registry.hpp"
#include "TCPTools.hpp"

std::shared_ptr<tcp::TcpConnection> tcp::TcpConnection::create(
  std::shared_ptr<boost::asio::ip::tcp::socket> pSocket,
  int id,
  msg::FORMAT format,
  const std::string& privateKey)
{
  return std::shared_ptr<tcp::TcpConnection>(
    new tcp::TcpConnection(pSocket, id, format, privateKey));
}

tcp::TcpConnection::TcpConnection(
  std::shared_ptr<boost::asio::ip::tcp::socket> pSocket,
  int id,
  msg::FORMAT format,
  const std::string& privateKey)
  : m_encrypted(!privateKey.empty()),
    m_pPrivateKey(tcp::createPrivateRSA(privateKey)),
    m_pSocket(pSocket),
    m_id(id),
    m_format(format),
    m_closedSignal(),
    m_inputBuffer(1024),
    m_handlers(std::make_shared<tcp::HandlerMap>()),
    m_pMessages(std::make_shared<std::map<std::string, msg::ResendMsg>>()),
    m_acq(std::function<void(std::string)>([
      handlers = m_handlers,
      format = m_format,
      m_pMessages = m_pMessages,
      m_pPrivateKey = m_pPrivateKey,
      m_encrypted = m_encrypted
    ](std::string input) {
      while (true)
      {
        auto optMsg = tcp::getNextStringMessage(input);
        if (!optMsg) return;
        GlobalRegistry::getRegistry().postToThreadPool([
          optMsg,
          handlers,
          format,
          m_pMessages,
          m_pPrivateKey,
          m_encrypted = m_encrypted
        ]() {
          msg::BaseMsg receivedMsg;
          auto msg = optMsg.get();
          if (!msg::parseString(receivedMsg, msg, format))
          {
            std::cout << "Could not parse msg" << std::endl;
            return;
          }
          if (m_encrypted)
          {
            char* decrypted = new char[1024];
            int result = RSA_private_decrypt(static_cast<int>(receivedMsg.msg().size()),
                                            (unsigned char*)receivedMsg.msg().c_str(),
                                            (unsigned char*)decrypted,
                                            m_pPrivateKey.get(),
                                            RSA_PKCS1_PADDING);
            if (result < 0)
            {
              logger::logError("TCPClient", "Failed to decypt message");
              return;
            }
            receivedMsg.msg(std::string(decrypted, result));
            delete[] decrypted;
          }

          auto msgSent = m_pMessages->find(receivedMsg.msgId());
          auto handle = handlers->get(receivedMsg.type());
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
}

void tcp::TcpConnection::startRead()
{
  m_pSocket->async_read_some(
    boost::asio::buffer(m_inputBuffer),
    [this](const boost::system::error_code& ec, std::size_t bytes_transferred) {
      handleRead(ec, bytes_transferred);
    });
}

void tcp::TcpConnection::ready()
{
  m_acq.ready();
}

void tcp::TcpConnection::handleWrite(const boost::system::error_code& ec,
                                     size_t)
{
  if (ec) m_closedSignal(m_id);
}

void tcp::TcpConnection::close()
{
  m_pSocket->close();
}

void tcp::TcpConnection::checkMsgs(
  const std::chrono::steady_clock::time_point& now)
{
  for (auto m = m_pMessages->begin(); m != m_pMessages->end();)
  {
    if (m->second.expireTime < now && m->second.retries > 2)
      m = m_pMessages->erase(m);
    else
      m++;
  }

  for (auto m : *m_pMessages)
  {
    if (m.second.expireTime < now) send<msg::BaseMsg>(m.second.msg);
  }
}

void tcp::TcpConnection::handleRead(const boost::system::error_code& ec,
                                    std::size_t bytes_transferred)
{
  if (!ec)
  {
    std::string toAdd(
      m_inputBuffer.begin(), m_inputBuffer.begin() + bytes_transferred);
    m_acq.add(toAdd);
    m_inputBuffer.clear();
    m_inputBuffer.resize(1024);

    startRead();
  }
  else
  {
    std::cout << "Closing socket: " << ec.message() << "\n";
    m_closedSignal(m_id);
  }
}

void tcp::TcpConnection::registerDisconnect(
  boost::signals2::signal<void(int)>::slot_type slot)
{
  m_closedSignal.connect(slot);
}

void tcp::TcpConnection::resend(const msg::BaseMsg& msg)
{
  auto pMessage = std::make_shared<std::string>(
    tcp::getProcessedString(msg::toString(msg, m_format)));
  m_pSocket->async_write_some(
    boost::asio::buffer(*pMessage, pMessage.get()->size()),
    [this, pMessage](auto a, auto b) { this->handleWrite(a, b); });
  (*m_pMessages)[msg.msgId()].expireTime =
    std::chrono::steady_clock::now() + std::chrono::seconds(10);
  (*m_pMessages)[msg.msgId()].retries++;
}