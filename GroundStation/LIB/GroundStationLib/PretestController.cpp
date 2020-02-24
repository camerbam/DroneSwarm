#include "PretestController.hpp"

#include "LoggerLib/Logger.hpp"
#include "MsgLib/FinishMsg.hpp"
#include "MsgLib/FinishRsp.hpp"
#include "MsgLib/FlightPathMsg.hpp"
#include "MsgLib/HitTargetMsg.hpp"
#include "MsgLib/HitTargetRsp.hpp"
#include "MsgLib/PingMsg.hpp"
#include "MsgLib/PingRsp.hpp"
#include "MsgLib/ReadyMsg.hpp"
#include "MsgLib/ReadyRsp.hpp"
#include "MsgLib/ReadyRspRsp.hpp"
#include "MsgLib/ZConfigMsg.hpp"
#include "UtilsLib/Utils.hpp"

namespace
{
  const std::string NAME("GroundStation");

  template <typename Msg>
  void sendMessage(tcp::TcpClient& client,
                   std::condition_variable& cv,
                   std::mutex& m)
  {
    client.send(Msg());
    std::unique_lock<std::mutex> lock(m);
    cv.wait(lock);
  }
}

ground::PretestController::PretestController(std::string host, std::string port)
  : m_pinged(false),
    m_cvs(4),
    m_ms(4),
    m_connections(),
    m_toReferee(host,
                port,
                msg::FORMAT::PROTOBUF,
                NAME,
                GlobalRegistry::getRegistry().getPretest() == 3)
{
  createRefereeMsgHandlers();
  m_toReferee.ready();
  sendMessage<msg::ReadyMsg>(m_toReferee, m_cvs[0], m_ms[0]);
  sendMessage<msg::HitTargetMsg>(m_toReferee, m_cvs[1], m_ms[1]);
  sendMessage<msg::FinishMsg>(m_toReferee, m_cvs[2], m_ms[2]);
  if (!m_pinged)
  {
    std::unique_lock<std::mutex> lock(m_ms[3]);
    m_cvs[3].wait(lock);
  }
}

ground::PretestController::~PretestController()
{
}

void ground::PretestController::createRefereeMsgHandlers()
{
  m_connections.push_back(m_toReferee.registerHandler<msg::ReadyRsp>(
    [this](const msg::ReadyRsp&, const std::string&) {
      logger::logInfo("Referee Controller", "Received ReadyRsp");
      m_toReferee.send(msg::ReadyRspRsp());
      m_cvs[0].notify_one();
    }));

  m_connections.push_back(m_toReferee.registerHandler<msg::FinishRsp>(
    [this](const msg::FinishRsp&, const std::string&) {
      logger::logInfo("Referee Controller", "Received FinishRsp");
      m_cvs[2].notify_one();
    }));

  m_connections.push_back(m_toReferee.registerHandler<msg::PingMsg>(
    [this](const msg::PingMsg&, const std::string& msgId) {
      m_toReferee.respond(msg::PingRsp(), msgId);
      logger::logInfo("Referee Controller", "Received Ping and responded");
      m_cvs[3].notify_one();
      m_pinged = true;
    }));

  m_connections.push_back(m_toReferee.registerHandler<msg::HitTargetRsp>(
    [this](const msg::HitTargetRsp&, const std::string&) {
      logger::logInfo("Referee Controller", "Received HitTargetRsp");
      m_cvs[1].notify_one();
    }));
}
