#include "DroneManager.hpp"

#include <boost/asio/ip/host_name.hpp>

#include "DroneMessagesLib/Messages/GoMessage.hpp"
#include "MsgLib/FinishMsg.hpp"
#include "MsgLib/FinishRsp.hpp"
#include "MsgLib/FlightPathMsg.hpp"
#include "MsgLib/FlightPathRsp.hpp"
#include "MsgLib/HitTargetMsg.hpp"
#include "MsgLib/TargetMsg.hpp"
#include "MsgLib/ZConfigMsg.hpp"
#include "MsgLib/ZConfigRsp.hpp"
#include "UtilsLib/Utils.hpp"

namespace
{
  void goX(double& x,
           std::queue<messages::Message_t>& messages,
           const double& px)
  {
    while (!utils::compareTwoDoubles(x, px))
    {
      auto xDiff = px - x;
      auto xDiffAbs = std::abs(xDiff);
      if (xDiffAbs < 20)
      {
        if (xDiff > 0)
        {
          messages.push(messages::ForwardMessage(xDiffAbs + 20));
          messages.push(messages::BackMessage(20));
        }
        else
        {
          messages.push(messages::BackMessage(xDiffAbs + 20));
          messages.push(messages::ForwardMessage(20));
        }
        x += xDiff;
      }
      else if (xDiffAbs < 120)
      {
        if (xDiff > 0)
          messages.push(messages::ForwardMessage(xDiffAbs));
        else
          messages.push(messages::BackMessage(xDiffAbs));
        x += xDiff;
      }
      else
      {
        if (xDiff > 0)
        {
          messages.push(messages::ForwardMessage(100));
          x += 100;
        }
        else
        {
          messages.push(messages::BackMessage(100));
          x -= 100;
        }
      }
    }
  }

  void goY(double& y,
           std::queue<messages::Message_t>& messages,
           const double& py)
  {
    while (!utils::compareTwoDoubles(y, py))
    {
      auto yDiff = py - y;
      auto yDiffAbs = std::abs(yDiff);
      if (yDiffAbs < 20)
      {
        if (yDiff > 0)
        {
          messages.push(messages::RightMessage(yDiffAbs + 20));
          messages.push(messages::LeftMessage(20));
        }
        else
        {
          messages.push(messages::LeftMessage(yDiffAbs + 20));
          messages.push(messages::RightMessage(20));
        }
        y += yDiff;
      }
      else if (yDiffAbs < 120)
      {
        if (yDiff > 0)
          messages.push(messages::RightMessage(yDiffAbs));
        else
          messages.push(messages::LeftMessage(yDiffAbs));
        y += yDiff;
      }
      else
      {
        if (yDiff > 0)
        {
          messages.push(messages::RightMessage(100));
          y += 100;
        }
        else
        {
          messages.push(messages::LeftMessage(100));
          y -= 100;
        }
      }
    }
  }
} // namespace

std::queue<messages::Message_t> drone::createFlightPath(
  double x, double y, const std::vector<msg::TargetMsg>& points)
{
  std::queue<messages::Message_t> toReturn;
  static int i;
  for (auto&& point : points)
  {
    i++;
    goX(x, toReturn, point.x());
    goY(y, toReturn, point.y());
  }
  return toReturn;
}

drone::DroneManager::DroneManager(const std::string& ipAddress,
                                  const unsigned short& dronePort,
                                  const unsigned short& droneStatusPort,
                                  const std::string& serverPort,
                                  const std::string& monitorPort,
                                  int startingY)
  : m_cv(),
    m_m(),
    m_logger("Drone Manager", "localhost", monitorPort),
    m_controller(m_logger, dronePort, droneStatusPort, ipAddress, startingY),
    m_client(boost::asio::ip::host_name(), serverPort),
    m_pathMutex(),
    m_flightPath(),
    m_targets(),
    m_connections(),
    m_sendThread(),
    m_toQuit(false),
    m_zConfig(100)
{
  m_connections.push_back(m_controller.registerForMid([this](int id) {
    if (m_targets.empty()) return;
    for (auto target = m_targets.begin(); target != m_targets.end(); target++)
    {
      if (utils::checkWithinDouble(target->x(), m_controller.getX(), 10) &&
          utils::checkWithinDouble(target->y(), m_controller.getY(), 10))
      {
        msg::HitTargetMsg msg(0, id, {target->x(), target->y()});
        m_client.send(msg);
        m_targets.erase(target);
        return;
      }
    }
  }));
  registerHandlers();
  m_client.ready();
}

drone::DroneManager::~DroneManager()
{
  {
    std::unique_lock<std::mutex> lock(m_m);
    m_cv.wait(lock);
  }
  m_client.close();
  if (m_sendThread && m_sendThread->joinable()) m_sendThread->join();
}

void drone::DroneManager::registerHandlers()
{
  m_connections.push_back(m_client.registerHandler<msg::FlightPathMsg>(
    [this](const msg::FlightPathMsg& msg, const std::string&) {
      m_logger.logInfo("DroneManager", "Received FlightPathMsg");

      auto path = createFlightPath(
        m_controller.getX(), m_controller.getY(), msg.targets());
      {
        std::lock_guard<std::mutex> l(m_pathMutex);
        std::swap(path, m_flightPath);
      }
      auto targets = msg.targets();
      std::swap(targets, m_targets);
      if (!m_controller.isFlying()) startMessages();
      msg::FlightPathRsp rsp;
      m_client.send(rsp);
    }));

  m_connections.push_back(m_client.registerHandler<msg::ZConfigMsg>(
    [this](const msg::ZConfigMsg& msg, const std::string&) {
      m_logger.logInfo("DroneManager", "Received ZConfigMsg");
      m_zConfig = msg.zAxis();
      m_client.send(msg::ZConfigRsp());
    }));

  m_connections.push_back(m_client.registerHandler<msg::FinishMsg>(
    [this](const msg::FinishMsg&, const std::string&) {
      m_logger.logInfo("DroneManager", "Received FinishMsg");
      {
        std::lock_guard<std::mutex> l(m_pathMutex);
        m_flightPath.push(messages::LandMessage());
      }
      m_client.send(msg::FinishRsp());
      m_toQuit = true;
    }));

  m_connections.push_back(m_client.registerHandler<msg::FinishMsg>(
    [this](const msg::FinishMsg&, const std::string&) {
      m_logger.logInfo("DroneManager", "Received FinishMsg");
      {
        std::lock_guard<std::mutex> l(m_pathMutex);
        m_flightPath.push(messages::LandMessage());
      }
      m_client.send(msg::FinishRsp());
      m_toQuit = true;
    }));
}

void drone::DroneManager::startMessages()
{
  static bool running(false);
  if (running) return;
  m_controller.sendMessage(messages::TakeoffMessage());
  m_controller.waitForStatusMsg();
  auto diff = 100 - m_zConfig;
  std::cout << "z" << m_controller.getZ();
  std::cout << "diff" << diff << std::endl;
  if (diff <= -20) m_controller.sendMessage(messages::UpMessage(-diff));
  if (diff >= 20) m_controller.sendMessage(messages::DownMessage(diff));
  m_sendThread = std::make_shared<std::thread>([this]() {
    while (!m_toQuit || !m_flightPath.empty())
    {
      if (m_flightPath.empty())
      {
        m_logger.logInfo("DroneManager", "No targets to go to");
        std::this_thread::sleep_for(std::chrono::seconds(1));
        continue;
      }
      auto toGoTo = m_flightPath.front();
      {
        std::lock_guard<std::mutex> l(m_pathMutex);
        m_flightPath.pop();
      }
      m_controller.sendMessage(toGoTo);
    }
    m_controller.stopRunning();
    m_cv.notify_one();
  });
}
