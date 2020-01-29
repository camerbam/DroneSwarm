
#include <condition_variable>

#include "TCPLib/TCPServer.hpp"

#include "MsgLib/FinishMsg.hpp"
#include "MsgLib/FlightPathMsg.hpp"
#include "MsgLib/TargetMsg.hpp"
#include "MsgLib/TargetRsp.hpp"
#include "MsgLib/ZConfigMsg.hpp"
#include "MsgLib/ZConfigRsp.hpp"
#include "UtilsLib/Utils.hpp"

namespace
{
  void drone1(std::shared_ptr<tcp::TcpConnection> connection,
              std::vector<boost::signals2::connection>& connections,
              std::vector<Target> targets,
              std::condition_variable& cv)
  {
    connections.push_back(connection->registerHandler<msg::ZConfigRsp>(
      [](const msg::ZConfigRsp&) {}));

    connections.push_back(connection->registerHandler<msg::TargetMsg>(
      [connection, &targets, &cv](const msg::TargetMsg& target) {
        auto nextTarget = targets.front();
        if (utils::compareTwoDoubles(nextTarget.getX(), target.point().x()) &&
            utils::compareTwoDoubles(nextTarget.getY(), target.point().y()) &&
            utils::compareTwoDoubles(nextTarget.getId(), target.id()))
        {
          std::cout << "correct" << std::endl;
          connection->send(msg::TargetRsp());
          targets.erase(targets.begin());
        }
        else
        {
          std::cout << "wrong place" << std::endl;
          connection->send(msg::TargetRsp("Wrong id, or wrong location"));
        }

        if (targets.empty())
        {
          connection->send(msg::FinishMsg());
          connection->close();
          cv.notify_one();
        }
      }));
    connection->ready();
  }

  void drone2(std::shared_ptr<tcp::TcpConnection> connection,
    std::vector<boost::signals2::connection>& connections,
    std::vector<Target> targets,
    std::condition_variable& cv)
  {
    connections.push_back(connection->registerHandler<msg::ZConfigRsp>(
      [](const msg::ZConfigRsp&) {}));

    connections.push_back(connection->registerHandler<msg::TargetMsg>(
      [connection, &targets, &cv](const msg::TargetMsg& target) {
      auto nextTarget = targets.front();
      if (utils::compareTwoDoubles(nextTarget.getX(), target.point().x()) &&
        utils::compareTwoDoubles(nextTarget.getY(), target.point().y()) &&
        utils::compareTwoDoubles(nextTarget.getId(), target.id()))
      {
        std::cout << "correct" << std::endl;
        connection->send(msg::TargetRsp());
        targets.erase(targets.begin());
      }
      else
      {
        std::cout << "wrong place" << std::endl;
        connection->send(msg::TargetRsp("Wrong id, or wrong location"));
      }

      if (targets.empty())
      {
        connection->send(msg::FinishMsg());
        connection->close();
        cv.notify_one();
      }
    }));
    connection->ready();
  }
}

int main()
{
  std::vector<boost::signals2::connection> connections;
  std::vector<std::shared_ptr<tcp::TcpConnection>> drones;
  GlobalRegistry::setRegistry();
  tcp::TcpServer server(65000);

  std::condition_variable cv;
  std::mutex m;

  std::vector<Target> targets;
  targets.emplace_back(100, 0, 2);
  targets.emplace_back(100, 100, 3);

  std::vector<Target> targets2;
  targets2.emplace_back(200, 100, 4);
  targets2.emplace_back(200, 0, 5);

  bool first(true);

  connections.push_back(server.registerConnection(
    [&connections, &drones, &server, &cv, &targets, &targets2, first](
      std::shared_ptr<tcp::TcpConnection> connection) {
      drones.push_back(connection);
      if (first)
        drone1(connection, connections, targets, cv);
      else
        drone2(connection, connections, targets2, cv);
    }));

  std::cout << "type ready when ready" << std::endl;
  std::string line;
  while (line != "ready")
    std::getline(std::cin, line);

  for (auto&& drone : drones)
  {
    std::cout << "sending" << std::endl;
    drone->send(msg::ZConfigMsg(150));
    drone->send(msg::FlightPathMsg({{100, 0}, {100, 100}, {200, 0}}));
  }

  {
    std::unique_lock<std::mutex> lock(m);
    cv.wait(lock);
  }
  {
    std::unique_lock<std::mutex> lock(m);
    cv.wait(lock);
  }

  server.close();

  return 0;
}
