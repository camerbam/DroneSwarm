#include "DroneManager.hpp"

#include <boost/asio/ip/host_name.hpp>

#include "DroneMessagesLib/Messages/GoMessage.hpp"
#include "MsgLib/FinishMsg.hpp"
#include "MsgLib/FinishRsp.hpp"
#include "MsgLib/FlightPathMsg.hpp"
#include "MsgLib/FlightPathRsp.hpp"
#include "MsgLib/Point.hpp"
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
      else if (xDiffAbs < 500)
      {
        if (xDiff > 0)
          messages.push(messages::ForwardMessage(xDiffAbs));
        else
          messages.push(messages::BackMessage(xDiffAbs));
        x += xDiff;
      }
      else if (xDiffAbs < 520)
      {
        if (xDiff > 0)
        {
          messages.push(messages::ForwardMessage(xDiffAbs - 20));
          messages.push(messages::ForwardMessage(20));
        }
        else
        {
          messages.push(messages::BackMessage(xDiffAbs - 20));
          messages.push(messages::BackMessage(20));
        }
        x += xDiff;
      }
      else
      {
        if (xDiff > 0)
        {
          messages.push(messages::ForwardMessage(500));
          x += 500;
        }
        else
        {
          messages.push(messages::BackMessage(500));
          x -= 500;
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
      else if (yDiffAbs < 500)
      {
        if (yDiff > 0)
          messages.push(messages::RightMessage(yDiffAbs));
        else
          messages.push(messages::LeftMessage(yDiffAbs));
        y += yDiff;
      }
      else if (yDiffAbs < 520)
      {
        if (yDiff > 0)
        {
          messages.push(messages::RightMessage(yDiffAbs - 20));
          messages.push(messages::RightMessage(20));
        }
        else
        {
          messages.push(messages::LeftMessage(yDiffAbs - 20));
          messages.push(messages::LeftMessage(20));
        }
        y += yDiff;
      }
      else
      {
        if (yDiff > 0)
        {
          messages.push(messages::RightMessage(500));
          y += 500;
        }
        else
        {
          messages.push(messages::LeftMessage(500));
          y -= 500;
        }
      }
    }
  }
}

std::queue<messages::Message_t> drone::createFlightPath(
  double x, double y, const std::vector<msg::Point>& points)
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
                                  const std::string& serverPort,
                                  const std::string& monitorPort,
                                  int startingY)
  : m_controller(ipAddress, startingY),
    m_client(boost::asio::ip::host_name(), serverPort),
    m_pathMutex(),
    m_flightPath(),
    m_points(),
    m_connections(),
    m_logger("Drone Manager", monitorPort),
    m_sendThread(),
    m_toQuit(false),
    m_zConfig(100)
{
  m_connections.push_back(m_controller.registerForMid([this](int id) {
    if (m_points.empty()) return;
    if (utils::checkWithinDouble(m_points[0].x(), m_controller.getX(), 10) &&
        utils::checkWithinDouble(m_points[0].y(), m_controller.getY(), 10))
    {
      msg::TargetMsg msg(id, {m_points[0].x(), m_points[0].y()});
      m_client.send(msg);
      m_points.erase(m_points.begin());
    }
  }));
  registerHandlers();
  m_client.ready();
}

drone::DroneManager::~DroneManager()
{
  m_toQuit = true;
  if (m_sendThread && m_sendThread->joinable()) m_sendThread->join();
}

void drone::DroneManager::registerHandlers()
{
  m_connections.push_back(m_client.registerHandler<msg::FlightPathMsg>(
    [this](const msg::FlightPathMsg& msg) {
      auto path = createFlightPath(
        m_controller.getX(), m_controller.getY(), msg.points());
      std::lock_guard<std::mutex> l(m_pathMutex);
      std::swap(path, m_flightPath);
      auto points = msg.points();
      std::swap(points, m_points);
      startMessages();
      msg::FlightPathRsp rsp;
      m_client.send(rsp);
    }));

  m_connections.push_back(m_client.registerHandler<msg::ZConfigMsg>(
    [this](const msg::ZConfigMsg& msg) {
      m_zConfig = msg.zAxis();
      m_client.send(msg::ZConfigRsp());
    }));

  m_connections.push_back(
    m_client.registerHandler<msg::FinishMsg>([this](const msg::FinishMsg&) {
      m_controller.sendMessage(messages::LandMessage());
      m_client.send(msg::FinishRsp());
    }));
}

void drone::DroneManager::startMessages()
{
  static bool running(false);
  if (running) return;
  m_controller.sendMessage(messages::TakeoffMessage());
  m_controller.waitForStatusMsg();
  auto diff = m_zConfig - m_controller.getZ();
  if (diff > 20) m_controller.sendMessage(messages::UpMessage(diff));
  m_sendThread = std::make_shared<std::thread>([this]() {
    while (!m_toQuit)
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
  });
}
