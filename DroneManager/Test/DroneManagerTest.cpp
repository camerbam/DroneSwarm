#include <boost/test/unit_test.hpp>

#include <iostream>

#include "DroneManagerLib/DroneManager.hpp"
#include "DroneMessagesLib/DroneStatusMessage.hpp"
#include "MsgLib/FinishMsg.hpp"
#include "MsgLib/FinishRsp.hpp"
#include "MsgLib/FlightPathMsg.hpp"
#include "MsgLib/FlightPathRsp.hpp"
#include "MsgLib/TargetMsg.hpp"
#include "MsgLib/ZConfigMsg.hpp"
#include "MsgLib/ZConfigRsp.hpp"
#include "RegistryLib/Registry.hpp"
#include "TCPLib/TCPServer.hpp"
#include "UDPLib/Response.hpp"
#include "UtilsLib/Utils.hpp"

namespace
{
  const std::string OK("ok");

  std::shared_ptr<std::thread> startSimulator()
  {
    return std::make_shared<std::thread>([]() {
      udp::UDPCommunicator c(8889);
      auto timeout = boost::posix_time::seconds(100);
      auto msg = c.receiveMessage(timeout);
      BOOST_CHECK(msg.getMessage() == "command");
      std::cout << msg.getMessage() << std::endl;
      c.sendMessage(OK, msg.getEndpoint());
      msg = c.receiveMessage(timeout);
      BOOST_CHECK(msg.getMessage() == "takeoff");
      std::cout << msg.getMessage() << std::endl;
      c.sendMessage(OK, msg.getEndpoint());
      msg = c.receiveMessage(timeout);
      BOOST_CHECK(msg.getMessage() == "up 200");
      std::cout << msg.getMessage() << std::endl;
      c.sendMessage(OK, msg.getEndpoint());
      msg = c.receiveMessage(timeout);
      BOOST_CHECK(msg.getMessage() == "right 30");
      std::cout << msg.getMessage() << std::endl;
      c.sendMessage(OK, msg.getEndpoint());
      msg = c.receiveMessage(timeout);
      BOOST_CHECK(msg.getMessage() == "forward 40");
      c.sendMessage(OK, msg.getEndpoint());
      auto statusEndpoint = msg.getEndpoint();
      statusEndpoint.port(8890);
      udp::UDPCommunicator c2;
      c2.sendMessage(
        messages::DroneStatusMessage().toString(-1, 30, 40, 200, 0, 270, 98, 5),
        statusEndpoint);
      c2.sendMessage(
        messages::DroneStatusMessage().toString(2, 30, 40, 200, 0, 270, 98, 5),
        statusEndpoint);
      msg = c.receiveMessage();
      BOOST_CHECK(msg.getMessage() == "land");
      c.sendMessage(OK, msg.getEndpoint());
    });
  }

  std::shared_ptr<std::thread> startServer(
    std::vector<boost::signals2::scoped_connection>& connections,
    std::condition_variable& startManager,
    std::mutex& m)
  {
    return std::make_shared<std::thread>([&startManager, &m, &connections]() {
      std::atomic<bool> zRsp(false);
      std::atomic<bool> flightRsp(false);
      {
        tcp::TcpServer server(65000);

        connections.push_back(server.registerConnection(
          [&connections, &startManager, &m, &server, &zRsp, &flightRsp](
            std::shared_ptr<tcp::TcpConnection> connection) {
            connections.push_back(connection->registerHandler<msg::ZConfigRsp>(
              [&zRsp](const msg::ZConfigRsp&) { zRsp = true; }));

            connections.push_back(
              connection->registerHandler<msg::FlightPathRsp>(
                [&flightRsp](const msg::FlightPathRsp&) { flightRsp = true; }));

            connections.push_back(connection->registerHandler<msg::TargetMsg>(
              [connection](const msg::TargetMsg& target) {
                BOOST_CHECK(target.id() == 2);
                BOOST_CHECK(utils::compareTwoDoubles(target.point().x(), 30));
                BOOST_CHECK(utils::compareTwoDoubles(target.point().y(), 40));
                msg::FinishMsg msg;
                connection->send(msg);
              }));

            connections.push_back(connection->registerHandler<msg::FinishRsp>(
              [connection, &server](const msg::FinishRsp&) {
                connection->close();
                server.close();
              }));

            startManager.notify_one();
            {
              std::unique_lock<std::mutex> lock(m);
              startManager.wait_for(lock, std::chrono::milliseconds(100));
            }

            connection->ready();
            msg::ZConfigMsg zMsg(200);
            connection->send(zMsg);

            msg::FlightPathMsg msg({{30, 40}});
            connection->send(msg);
          }));
      }
      BOOST_CHECK(zRsp);
      BOOST_CHECK(flightRsp);
    });
  }
}

BOOST_AUTO_TEST_CASE(DRONE_MANAGER_TEST)
{
  auto drone = startSimulator();

  std::condition_variable startManager;
  std::mutex m;
  std::vector<boost::signals2::scoped_connection> connections;
  auto t1 =
    startServer(connections, startManager, m);

  {
    std::unique_lock<std::mutex> lock(m);
    startManager.wait_for(lock, std::chrono::milliseconds(100));
  }
  drone::DroneManager manager("127.0.0.1", "65000", "13000");

  startManager.notify_one();

  drone->join();
  t1->join();
}

BOOST_AUTO_TEST_CASE(DRONE_MANAGER_PATH_TEST)
{
  std::vector<msg::Point> points;
  points.push_back({ 50, 50 }); // 2
  points.push_back({ 0, 0 }); // 2
  points.push_back({ 10, 10 }); // 4
  points.push_back({ 0, 0 }); // 4
  points.push_back({ 540, 540 }); // 4
  points.push_back({ 0, 0 }); // 4
  points.push_back({ 510, 510 }); // 4
  points.push_back({ 0, 0 }); // 4
  auto rsp = drone::createFlightPath(0, 0, points);

  messages::RightMessage().getArgument();
  BOOST_REQUIRE(rsp.size() == 28);
  BOOST_CHECK(utils::compareTwoDoubles(boost::get<messages::RightMessage>(rsp.front()).getArgument(), 50));
  rsp.pop();
  BOOST_CHECK(utils::compareTwoDoubles(boost::get<messages::ForwardMessage>(rsp.front()).getArgument(), 50));
  rsp.pop();
  BOOST_CHECK(utils::compareTwoDoubles(boost::get<messages::LeftMessage>(rsp.front()).getArgument(), 50));
  rsp.pop();
  BOOST_CHECK(utils::compareTwoDoubles(boost::get<messages::BackMessage>(rsp.front()).getArgument(), 50));
  rsp.pop();


  BOOST_CHECK(utils::compareTwoDoubles(boost::get<messages::RightMessage>(rsp.front()).getArgument(), 30));
  rsp.pop();
  BOOST_CHECK(utils::compareTwoDoubles(boost::get<messages::LeftMessage>(rsp.front()).getArgument(), 20));
  rsp.pop();
  BOOST_CHECK(utils::compareTwoDoubles(boost::get<messages::ForwardMessage>(rsp.front()).getArgument(), 30));
  rsp.pop();
  BOOST_CHECK(utils::compareTwoDoubles(boost::get<messages::BackMessage>(rsp.front()).getArgument(), 20));
  rsp.pop();


  BOOST_CHECK(utils::compareTwoDoubles(boost::get<messages::LeftMessage>(rsp.front()).getArgument(), 30));
  rsp.pop();
  BOOST_CHECK(utils::compareTwoDoubles(boost::get<messages::RightMessage>(rsp.front()).getArgument(), 20));
  rsp.pop();
  BOOST_CHECK(utils::compareTwoDoubles(boost::get<messages::BackMessage>(rsp.front()).getArgument(), 30));
  rsp.pop();
  BOOST_CHECK(utils::compareTwoDoubles(boost::get<messages::ForwardMessage>(rsp.front()).getArgument(), 20));
  rsp.pop();

  BOOST_CHECK(utils::compareTwoDoubles(boost::get<messages::RightMessage>(rsp.front()).getArgument(), 500));
  rsp.pop();
  BOOST_CHECK(utils::compareTwoDoubles(boost::get<messages::RightMessage>(rsp.front()).getArgument(), 40));
  rsp.pop();
  BOOST_CHECK(utils::compareTwoDoubles(boost::get<messages::ForwardMessage>(rsp.front()).getArgument(), 500));
  rsp.pop();
  BOOST_CHECK(utils::compareTwoDoubles(boost::get<messages::ForwardMessage>(rsp.front()).getArgument(), 40));
  rsp.pop();

  BOOST_CHECK(utils::compareTwoDoubles(boost::get<messages::LeftMessage>(rsp.front()).getArgument(), 500));
  rsp.pop();
  BOOST_CHECK(utils::compareTwoDoubles(boost::get<messages::LeftMessage>(rsp.front()).getArgument(), 40));
  rsp.pop();
  BOOST_CHECK(utils::compareTwoDoubles(boost::get<messages::BackMessage>(rsp.front()).getArgument(), 500));
  rsp.pop();
  BOOST_CHECK(utils::compareTwoDoubles(boost::get<messages::BackMessage>(rsp.front()).getArgument(), 40));
  rsp.pop();

  BOOST_CHECK(utils::compareTwoDoubles(boost::get<messages::RightMessage>(rsp.front()).getArgument(), 490));
  rsp.pop();
  BOOST_CHECK(utils::compareTwoDoubles(boost::get<messages::RightMessage>(rsp.front()).getArgument(), 20));
  rsp.pop();
  BOOST_CHECK(utils::compareTwoDoubles(boost::get<messages::ForwardMessage>(rsp.front()).getArgument(), 490));
  rsp.pop();
  BOOST_CHECK(utils::compareTwoDoubles(boost::get<messages::ForwardMessage>(rsp.front()).getArgument(), 20));
  rsp.pop();

  BOOST_CHECK(utils::compareTwoDoubles(boost::get<messages::LeftMessage>(rsp.front()).getArgument(), 490));
  rsp.pop();
  BOOST_CHECK(utils::compareTwoDoubles(boost::get<messages::LeftMessage>(rsp.front()).getArgument(), 20));
  rsp.pop();
  BOOST_CHECK(utils::compareTwoDoubles(boost::get<messages::BackMessage>(rsp.front()).getArgument(), 490));
  rsp.pop();
  BOOST_CHECK(utils::compareTwoDoubles(boost::get<messages::BackMessage>(rsp.front()).getArgument(), 20));
  rsp.pop();
}
