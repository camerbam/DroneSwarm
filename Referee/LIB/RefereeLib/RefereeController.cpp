#include "RefereeController.hpp"

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

namespace
{
  std::vector<msg::TargetMsg> convertTargets(const std::vector<Target>& targets)
  {
    std::vector<msg::TargetMsg> toReturn;
    for (auto&& t : targets)
      toReturn.emplace_back(t);
    return toReturn;
  }
} // namespace

referee::RefereeController::RefereeController(unsigned short port,
                                              msg::FORMAT format)
  : m_pCtx(std::make_shared<boost::asio::io_context>()),
    m_optCork(*m_pCtx),
    m_timer(*m_pCtx),
    m_server(port, format, GlobalRegistry::getRegistry().isEncypted()),
    m_connections(),
    m_gameManagers(),
    m_iocThread([pCtx = m_pCtx]() { pCtx->run(); })
{
  m_connections.push_back(m_server.registerConnection(
    [this](std::shared_ptr<tcp::TcpConnection> connection) {
      logger::logInfo("RefereeController", "New connection");
      m_connections.push_back(connection->registerHandler<msg::ReadyMsg>(
        [this, connection](const msg::ReadyMsg&, const std::string&) {
          logger::logInfo("RefereeController", "received ReadyMsg");
          int i = static_cast<int>(m_gameManagers.size());
          auto& game = m_gameManagers[i];
          std::vector<Target> targets = game.startGame();
          connection->send(msg::ReadyRsp(i, convertTargets(targets)));
          m_gameManagers[i].addMsg(msg::ReadyMsg::name());
        }));

      m_connections.push_back(connection->registerHandler<msg::ReadyRspRsp>(
        [this](const msg::ReadyRspRsp& msg, const std::string&) {
          logger::logInfo("RefereeController", "received ReadyRspRsp");

          auto game = m_gameManagers.find(msg.gameId());
          if (game == m_gameManagers.end())
            logger::logError(
              "RefereeController", "Received ReadyRspRsp to invalid game");
          m_gameManagers[msg.gameId()].addMsg(msg::ReadyRspRsp::name());
        }));

      m_connections.push_back(connection->registerHandler<msg::PingRsp>(
        [this](const msg::PingRsp& msg, const std::string&) {
        logger::logInfo("RefereeController", "received PingRsp");
        m_gameManagers[msg.gameId()].addMsg(msg::PingRsp::name());
      }));

      m_connections.push_back(connection->registerHandler<msg::HitTargetMsg>(
        [this, connection](
          const msg::HitTargetMsg& msg, const std::string& msgId) {
          logger::logInfo("RefereeController", "received HitTargetMsg");

          auto& game = m_gameManagers[msg.gameId()];

          Target target(msg.target().x(), msg.target().y(), msg.id());
          auto hit = game.hitTarget(target);

          auto toSend = msg::HitTargetRsp();

          if (hit)
          {
            logger::logInfo(
              "RefereeController",
              "successfully hit " + std::to_string(target.getId()));
            toSend.success(true);
            if (game.isDone())
            {
              toSend.complete(true);
              logger::logInfo(
                "RefereeController", "successfully hit all targets");
              game.finish();
            }
            toSend.badTargets(convertTargets(hit.get().targetsToAdd));
            toSend.badTargets(convertTargets(hit.get().targetsToRemove));
          }
          connection->respond(toSend, msgId);

          game.addMsg(msg::HitTargetMsg::name());
        }));

      m_connections.push_back(connection->registerHandler<msg::FinishMsg>(
        [this, connection](
          const msg::FinishMsg& msg, const std::string& msgId) {
          logger::logInfo("RefereeController", "received FinishMsg");
          auto& game = m_gameManagers[msg.gameId()];
          auto targets = game.finish();
          connection->respond(msg::FinishRsp(convertTargets(targets)), msgId);
          game.addMsg(msg::FinishMsg::name());
        }));

      connection->ready();
    }));
  m_timer.expires_after(boost::asio::chrono::seconds(1));
  m_timer.async_wait([this](const boost::system::error_code& e) {
    if (!e) sendPing();
  });
}

referee::RefereeController::~RefereeController()
{
  m_optCork = boost::none;
  m_pCtx->stop();
  if (m_iocThread.joinable()) m_iocThread.join();
  m_server.close();
  m_timer.cancel();
}

void referee::RefereeController::sendPing()
{
  m_timer.expires_after(boost::asio::chrono::seconds(10));
  m_timer.async_wait([this](const boost::system::error_code& e) {
    if (!e) sendPing();
  });
  logger::logInfo("Referee Controller", "sending ping msg");
  m_server.sendToAll(msg::PingMsg());
}
