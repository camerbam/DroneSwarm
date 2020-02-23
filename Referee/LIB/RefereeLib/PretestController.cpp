#include "PretestController.hpp"

#include <iomanip>
#include <sstream>

#include "LoggerLib/Logger.hpp"
#include "MsgLib/FinishMsg.hpp"
#include "MsgLib/FinishRsp.hpp"
#include "MsgLib/HitTargetMsg.hpp"
#include "MsgLib/HitTargetRsp.hpp"
#include "MsgLib/PingMsg.hpp"
#include "MsgLib/PingRsp.hpp"
#include "MsgLib/ReadyMsg.hpp"
#include "MsgLib/ReadyRsp.hpp"
#include "MsgLib/ReadyRspRsp.hpp"
#include "TCPLib/TCPConnection.hpp"

namespace
{
  template <typename Rsp>
  referee::MSG_STATUS handleMsg(
    const std::shared_ptr<tcp::TcpConnection>& connection)
  {
    auto randomNumber = rand() % 100;
    std::cout << randomNumber << std::endl;
    if (randomNumber > 80)
    {
      connection->send(Rsp());
      connection->send(Rsp());
      return referee::MSG_STATUS::RECEIVED;
    }
    else if (randomNumber > 60)
    {
      return referee::MSG_STATUS::WAITING;
    }
    else if (randomNumber > 40)
    {
      std::this_thread::sleep_for(std::chrono::seconds(randomNumber % 10));
      connection->send(Rsp());
      return referee::MSG_STATUS::RECEIVED;
    }
    else
    {
      connection->send(Rsp());
      return referee::MSG_STATUS::RECEIVED;
    }
  }
}

referee::PretestController::PretestController(unsigned short port,
                                              msg::FORMAT format,
                                              size_t pretest)
  : m_cv(),
    m_m(),
    m_server(port, format, pretest == 3),
    m_connections(),
    m_msgs()
{
  m_msgs.emplace("ReadyMsg", MSG_STATUS::WAITING);
  m_msgs.emplace("ReadyRspRsp", MSG_STATUS::WAITING);
  m_msgs.emplace("HitTargetMsg", MSG_STATUS::WAITING);
  m_msgs.emplace("FinishMsg", MSG_STATUS::WAITING);
  m_msgs.emplace("PingRsp", MSG_STATUS::WAITING);
  if (pretest == 1 || pretest == 3)
    pretest1();
  else
    pretest2();
}

referee::PretestController::~PretestController()
{
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  m_server.close();
}

int referee::PretestController::execute()
{
  {
    auto start = std::chrono::steady_clock::now();
    std::unique_lock<std::mutex> lock(m_m);
    m_cv.wait_for(lock, std::chrono::minutes(1));
    if (start + std::chrono::minutes(1) < std::chrono::steady_clock::now())
      std::cout << "Exiting as timeout reached" << std::endl;
  }
  // TODO: Hacky
  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  bool success(true);
  std::stringstream ss;
  ss << std::endl << "************* RESULTS *************" << std::endl;
  for (auto&& m : m_msgs)
  {
    ss << std::setw(20) << std::left << m.first << std::setw(5) << std::right;
    switch (m.second)
    {
    case MSG_STATUS::WAITING:
      success = false;
      ss << ": Never Received" << std::endl;
      break;
    case MSG_STATUS::RECEIVED:
      ss << ": Sucesses" << std::endl;
      break;
    case MSG_STATUS::SEND_AGAIN:
      success = false;
      ss << ": Received, but should have been sent again" << std::endl;
      break;
    }
  }
  logger::logInfo("Pretest", ss.str());
  return !success;
}

void referee::PretestController::pretest1()
{
  m_connections.push_back(m_server.registerConnection([this](
    std::shared_ptr<tcp::TcpConnection> connection) {
    connection->send(msg::PingMsg());
    logger::logInfo("RefereeController", "New connection");
    m_connections.push_back(connection->registerHandler<msg::ReadyMsg>(
      [this, connection](const msg::ReadyMsg&, const std::string&) {
        logger::logInfo("RefereeController", "received ReadyMsg");
        m_msgs["ReadyMsg"] = MSG_STATUS::RECEIVED;
        connection->send(msg::ReadyRsp());
      }));

    m_connections.push_back(connection->registerHandler<msg::ReadyRspRsp>(
      [this](const msg::ReadyRspRsp& msg, const std::string&) {
        logger::logInfo("RefereeController", "received ReadyRspRsp");
        m_msgs["ReadyRspRsp"] = MSG_STATUS::RECEIVED;
      }));

    m_connections.push_back(
      connection->registerHandler<msg::HitTargetMsg>([this, connection](
        const msg::HitTargetMsg& msg, const std::string& msgId) {
        logger::logInfo("RefereeController", "received HitTargetMsg");
        m_msgs["HitTargetMsg"] = MSG_STATUS::RECEIVED;
        connection->send(msg::HitTargetRsp());
      }));

    m_connections.push_back(connection->registerHandler<msg::FinishMsg>(
      [this, connection](const msg::FinishMsg& msg, const std::string& msgId) {
        logger::logInfo("RefereeController", "received FinishMsg");
        m_msgs["FinishMsg"] = MSG_STATUS::RECEIVED;
        connection->send(msg::FinishRsp());
        m_cv.notify_one();
      }));

    m_connections.push_back(connection->registerHandler<msg::PingRsp>(
      [this, connection](const msg::PingRsp& msg, const std::string& msgId) {
        logger::logInfo("RefereeController", "received PingRsp");
        m_msgs["PingRsp"] = MSG_STATUS::RECEIVED;
      }));

    connection->ready();
  }));
}

void referee::PretestController::pretest2()
{
  m_connections.push_back(m_server.registerConnection([this](
    std::shared_ptr<tcp::TcpConnection> connection) {
    connection->send(msg::PingMsg());
    srand((unsigned int)time(NULL));
    logger::logInfo("RefereeController", "New connection");
    m_connections.push_back(connection->registerHandler<msg::ReadyMsg>(
      [this, connection](const msg::ReadyMsg&, const std::string&) {
        logger::logInfo("RefereeController", "received ReadyMsg");
        m_msgs["ReadyMsg"] = handleMsg<msg::ReadyRsp>(connection);

      }));

    m_connections.push_back(connection->registerHandler<msg::ReadyRspRsp>(
      [this](const msg::ReadyRspRsp& msg, const std::string&) {
        logger::logInfo("RefereeController", "received ReadyRspRsp");
        m_msgs["ReadyRspRsp"] = MSG_STATUS::RECEIVED;
      }));

    m_connections.push_back(
      connection->registerHandler<msg::HitTargetMsg>([this, connection](
        const msg::HitTargetMsg& msg, const std::string& msgId) {
        logger::logInfo("RefereeController", "received HitTargetMsg");
        m_msgs["HitTargetMsg"] = handleMsg<msg::HitTargetRsp>(connection);
      }));

    m_connections.push_back(connection->registerHandler<msg::FinishMsg>(
      [this, connection](const msg::FinishMsg& msg, const std::string& msgId) {
        logger::logInfo("RefereeController", "received FinishMsg");
        m_msgs["FinishMsg"] = MSG_STATUS::RECEIVED;
        connection->send(msg::FinishRsp());
        m_cv.notify_one();
      }));

    m_connections.push_back(connection->registerHandler<msg::PingRsp>(
      [this, connection](const msg::PingRsp& msg, const std::string& msgId) {
        logger::logInfo("RefereeController", "received PingRsp");
        m_msgs["PingRsp"] = MSG_STATUS::RECEIVED;
      }));

    connection->ready();
  }));
}
