
#include <condition_variable>

#include "TCPLib/TCPServer.hpp"

#include "MsgLib/FinishMsg.hpp"
#include "MsgLib/FlightPathMsg.hpp"
#include "MsgLib/HitTargetMsg.hpp"
#include "MsgLib/HitTargetRsp.hpp"
#include "MsgLib/ZConfigMsg.hpp"
#include "MsgLib/ZConfigRsp.hpp"
#include "UtilsLib/Utils.hpp"

namespace
{
  void drone1(std::shared_ptr<tcp::TcpConnection> connection,
              std::vector<boost::signals2::connection>& connections,
              std::vector<Target>& targets,
              std::condition_variable& cv)
  {
    connections.push_back(connection->registerHandler<msg::ZConfigRsp>(
      [](const msg::ZConfigRsp&, const std::string&) {}));

    connections.push_back(connection->registerHandler<msg::HitTargetMsg>(
      [connection, &targets, &cv](
        const msg::HitTargetMsg& target, const std::string&) {
        auto nextTarget = targets.front();
        if (nextTarget.getX() == target.target().x() &&
            nextTarget.getY() == target.target().y() &&
            nextTarget.getId() == target.id())
        {
          std::cout << "correct" << std::endl;
          connection->send(msg::HitTargetRsp(true, false, {}, {}));
          targets.erase(targets.begin());
        }
        else
        {
          std::cout << target.id() << " " << nextTarget.getId();
          std::cout << "wrong place" << std::endl;
          connection->send(msg::HitTargetRsp(false, false, {}, {}));
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
              std::vector<Target>& targets,
              std::condition_variable& cv)
  {
    connections.push_back(connection->registerHandler<msg::ZConfigRsp>(
      [](const msg::ZConfigRsp&, const std::string&) {}));

    connections.push_back(connection->registerHandler<msg::HitTargetMsg>(
      [connection, &targets, &cv](
        const msg::HitTargetMsg& target, const std::string&) {
        auto nextTarget = targets.front();
        if (nextTarget.getX() == target.target().x() &&
            nextTarget.getY() == target.target().y() &&
            nextTarget.getId() == target.id())
        {
          std::cout << "correct" << std::endl;
          connection->send(msg::HitTargetRsp(true, false, {}, {}));
          targets.erase(targets.begin());
        }
        else
        {
          std::cout << target.id() << " " << nextTarget.getId() << std::endl;
          std::cout << target.target().x() << " " << nextTarget.getX()
                    << std::endl;
          std::cout << target.target().y() << " " << nextTarget.getY()
                    << std::endl;
          std::cout << "wrong place" << std::endl;
          connection->send(msg::HitTargetRsp(false, false, {}, {}));
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
} // namespace

int main()
{
  std::vector<boost::signals2::connection> connections;
  std::vector<std::shared_ptr<tcp::TcpConnection>> drones;
  GlobalRegistry::setRegistry();
  tcp::TcpServer server(65001);

  std::condition_variable cv;
  std::mutex m;

  std::vector<Target> targets;
  targets.emplace_back(100, 0, 1);
  targets.emplace_back(100, 100, 2);

  std::vector<Target> targets2;
  targets2.emplace_back(200, 100, 3);
  targets2.emplace_back(200, 0, 4);

  bool first(true);

  connections.push_back(server.registerConnection(
    [&connections, &drones, &server, &cv, &targets, &targets2, &first](
      std::shared_ptr<tcp::TcpConnection> connection) {
      drones.push_back(connection);
      if (first)
      {
        drone1(connection, connections, targets, cv);
        first = false;
      }
      else
        drone2(connection, connections, targets2, cv);
    }));

  std::cout << "type ready when ready" << std::endl;
  std::string line;
  while (line != "ready")
    std::getline(std::cin, line);

  int height = 60;
  for (auto&& drone : drones)
  {
    std::cout << "sending" << std::endl;
    drone->send(msg::ZConfigMsg(height));
    height += 20;
  }

  drones[0]->send(msg::FlightPathMsg({{100, 0}, {100, 100}}));
  drones[1]->send(msg::FlightPathMsg({{200, 100}, {200, 0}}));

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
