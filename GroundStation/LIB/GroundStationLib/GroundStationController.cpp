#include "GroundStationController.hpp"

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
#include "MsgLib/ZConfigMsg.hpp"
#include "UtilsLib/Utils.hpp"

namespace
{
  const std::string NAME("GroundStation");
}

ground::GroundStationController::GroundStationController(std::string host,
                                                         std::string port)
  : m_cv(),
    m_m(),
    m_gameId(),
    m_connections(),
    m_targets(),
    m_assignedTargets(),
    m_droneServer(65001),
    m_idleDrones(),
    m_busyDrones(),
    m_toReferee(host,
                port,
                msg::FORMAT::PROTOBUF,
                NAME,
                GlobalRegistry::getRegistry().isEncypted()),
    m_logger(NAME, "localhost", "13000")
{
  createRefereeMsgHandlers();
  m_connections.push_back(m_droneServer.registerConnection(
    [this](std::shared_ptr<tcp::TcpConnection> drone) {
      createDroneMsgHandlers(drone);
      drone->ready();
    }));
  m_toReferee.ready();
}

ground::GroundStationController::~GroundStationController()
{
  {
    std::unique_lock<std::mutex> lock(m_m);
    m_cv.wait(lock);
    m_droneServer.close();
  }
}

void ground::GroundStationController::start()
{
  m_logger.logInfo("Ground Station", "Starting the game");
  m_toReferee.send(msg::ReadyMsg());
}

void ground::GroundStationController::createRefereeMsgHandlers()
{
  m_connections.push_back(m_toReferee.registerHandler<msg::ReadyRsp>(
    [this](const msg::ReadyRsp& msg, const std::string&) {
      m_logger.logInfo("Referee Controller", "Received ReadyRsp");
      m_gameId = msg.gameId();
      m_targets = msg.targets();

      int start = 100;
      for (auto&& drone : m_idleDrones)
      {
        drone->send(msg::ZConfigMsg(start));
        start += 20;
      }

      assignTargets();
    }));

  m_connections.push_back(m_toReferee.registerHandler<msg::FinishRsp>(
    [this](const msg::FinishRsp& msg, const std::string&) {
      m_logger.logInfo("Referee Controller", "Received FinishRsp");
      auto targets = msg.targets();
      if (targets.empty())
        m_logger.logInfo("GroundStation", "Completed all targets");
      else
        m_logger.logError(
          "GroundStation",
          "Missed " + std::to_string(targets.size()) + " targets");

      for (auto&& drone : m_idleDrones)
        drone->send(msg::FinishMsg());
      for (auto&& drone : m_busyDrones)
        drone->send(msg::FinishMsg());
      m_connections.clear();
      m_toReferee.close();
      m_cv.notify_one();
    }));

  m_connections.push_back(m_toReferee.registerHandler<msg::PingMsg>(
    [this](const msg::PingMsg&, const std::string& msgId) {
      m_toReferee.respond(msg::PingRsp(m_gameId), msgId);
      m_logger.logInfo("Referee Controller", "Received Ping and responded");
    }));

  m_connections.push_back(m_toReferee.registerHandler<msg::HitTargetRsp>(
    [this](const msg::HitTargetRsp& msg, const std::string&) {
      m_logger.logInfo("Referee Controller", "Received HitTargetRsp");
      if (!msg.success()) m_toReferee.send(msg::FinishMsg(m_gameId));

      if (!msg.success() || msg.complete())
      {
        for (auto&& drone : m_idleDrones)
          drone->send(msg::FinishMsg());
        for (auto&& drone : m_busyDrones)
          drone->send(msg::FinishMsg());
        m_connections.clear();
        m_toReferee.close();
        m_cv.notify_one();
        return;
      }
      auto newTargets = msg.newTargets();
      if (!newTargets.empty())
      {
        m_targets.insert(m_targets.end(), newTargets.begin(), newTargets.end());
      }

      for (auto&& target : msg.badTargets())
      {
        auto val =
          std::find_if(m_targets.begin(),
                       m_targets.end(),
                       [&target](const msg::TargetMsg& msg) {
                         return msg.x() == target.x() && msg.y() == target.y();
                       });
        if (val != m_targets.end())
          m_targets.erase(val);
        else
        {
          auto val1 = std::find_if(m_assignedTargets.begin(),
                                   m_assignedTargets.end(),
                                   [&target](const msg::TargetMsg& msg) {
                                     return msg.x() == target.x() &&
                                            msg.y() == target.y();
                                   });
          if (val1 != m_assignedTargets.end()) m_assignedTargets.erase(val1);
        }
      }

      assignTargets();
    }));
}

void ground::GroundStationController::createDroneMsgHandlers(
  std::shared_ptr<tcp::TcpConnection> drone)
{
  m_connections.push_back(drone->registerHandler<msg::HitTargetMsg>(
    [this, drone](const msg::HitTargetMsg& msg, const std::string&) {
      m_logger.logInfo("Referee Controller", "Received HitTargetMsg");
      for (auto target = m_assignedTargets.begin();
           target != m_assignedTargets.end();
           target++)
      {
        if (utils::checkWithin(msg.target().x(), target->x(), 20) &&
            utils::checkWithin(msg.target().y(), target->y(), 20))
        {
          m_toReferee.send(msg::HitTargetMsg(m_gameId, msg.id(), *target));
          m_assignedTargets.erase(target);
          if (!m_targets.empty())
          {
            drone->send(msg::FlightPathMsg({m_targets[0]}));
            m_assignedTargets.push_back(m_targets[0]);
            m_targets.erase(m_targets.begin());
          }
          else
          {
            m_idleDrones.push_back(drone);
            m_busyDrones.erase(
              std::find(m_busyDrones.begin(), m_busyDrones.end(), drone));
          }
          break;
        }
      }
      drone->send(msg::HitTargetRsp());
    }));

  m_idleDrones.push_back(drone);
}

void ground::GroundStationController::assignTargets()
{
  auto toRemove = m_targets.size();
  for (auto&& drone : m_idleDrones)
  {
    if (!m_targets.empty())
    {
      drone->send(msg::FlightPathMsg({m_targets[0]}));
      m_assignedTargets.push_back(m_targets[0]);
      m_targets.erase(m_targets.begin());
      m_busyDrones.push_back(drone);
    }
  }
  m_idleDrones.erase(
    m_idleDrones.begin(),
    m_idleDrones.begin() + std::min(toRemove, m_idleDrones.size()));
}
