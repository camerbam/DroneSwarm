
#include <condition_variable>

#include "TCPLib/TCPServer.hpp"

#include "MsgLib/FinishMsg.hpp"
#include "MsgLib/FlightPathMsg.hpp"
#include "MsgLib/TargetMsg.hpp"
#include "MsgLib/TargetRsp.hpp"
#include "MsgLib/ZConfigMsg.hpp"
#include "MsgLib/ZConfigRsp.hpp"
#include "UtilsLib/Utils.hpp"

int main()
{
  std::vector<boost::signals2::connection> connections;
  std::vector<std::shared_ptr<tcp::TcpConnection>> drones;
  GlobalRegistry::setRegistry();
  tcp::TcpServer server(65000);

  std::condition_variable cv;
  std::mutex m;

  std::vector<Target> targets;
  targets.emplace_back(100, 0, 1);
  targets.emplace_back(100, 100, 2);
  targets.emplace_back(50, 0, 3);

  connections.push_back(
    server.registerConnection([&connections, &drones, &server, &cv,&targets](
      std::shared_ptr<tcp::TcpConnection> connection) {
      drones.push_back(connection);

      connections.push_back(connection->registerHandler<msg::ZConfigRsp>(
        [](const msg::ZConfigRsp&) {
        std::cout << "received" << std::endl;
      }));

      connections.push_back(connection->registerHandler<msg::TargetMsg>(
        [connection, &server, &targets, &cv](const msg::TargetMsg& target) {
          std::cout << "received" << std::endl;
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
            server.close();
            cv.notify_all();
          }
        }));
      connection->ready();
    }));

  std::cout << "type ready when ready" << std::endl;
  std::string line;
  while (line != "ready")
    std::getline(std::cin, line);

  for (auto&& drone : drones)
  {
    std::cout << "sending" << std::endl;
    drone->send(msg::ZConfigMsg(150));
    drone->send(msg::FlightPathMsg({{100, 0}, {100, 100}, {50, 0}}));
  }

  std::unique_lock<std::mutex> lock(m);
  cv.wait(lock);

  return 0;
}
