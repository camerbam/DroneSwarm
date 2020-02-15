#include "TCPConnection.hpp"

#include <iostream>

#include <boost/asio.hpp>
#include <boost/asio/write.hpp>

#include "MsgLib/BaseMsg.hpp"
#include "RegistryLib/Registry.hpp"
#include "TCPTools.hpp"

std::shared_ptr<tcp::TcpConnection> tcp::TcpConnection::create(
  std::shared_ptr<boost::asio::ip::tcp::socket> pSocket,
  int id,
  msg::FORMAT format)
{
  return std::shared_ptr<tcp::TcpConnection>(
    new tcp::TcpConnection(pSocket, id, format));
}

tcp::TcpConnection::TcpConnection(
  std::shared_ptr<boost::asio::ip::tcp::socket> pSocket,
  int id,
  msg::FORMAT format)
  : m_pSocket(pSocket),
    m_id(id),
    m_format(format),
    m_closedSignal(),
    m_inputBuffer(1024),
    m_handlers(std::make_shared<tcp::HandlerMap>()),
    m_pMessages(std::make_shared<std::map<std::string, msg::ResendMsg>>()),
    m_acq(std::function<void(std::string)>(
      [ handlers = m_handlers, format = m_format, m_pMessages = m_pMessages ](
        std::string input) {
        while (true)
        {
          auto optMsg = tcp::getNextStringMessage(input);
          if (!optMsg) return;
          GlobalRegistry::getRegistry().postToThreadPool(
            [optMsg, handlers, format, m_pMessages]() {
              msg::BaseMsg receivedMsg;
              auto msg = optMsg.get();
              if (!msg::parseString(receivedMsg, msg, format))
              {
                std::cout << "Could not parse msg" << std::endl;
                return;
              }
              auto msgSent = m_pMessages->find(receivedMsg.msgId());
              auto handle = handlers->get(receivedMsg.type());
              if (handle)
                handle->execute(receivedMsg.msg(), format, receivedMsg.msgId());
              if (msgSent != m_pMessages->end()) m_pMessages->erase(msgSent);
              else if (!handle)
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
  for (auto m : *m_pMessages)
    if (m.second.second < now) send<msg::BaseMsg>(m.second.first);
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
  (*m_pMessages)[msg.msgId()].second =
    std::chrono::steady_clock::now() + std::chrono::seconds(10);
}