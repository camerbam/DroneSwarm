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
      auto msg = c.receiveMessage(boost::posix_time::seconds(1));
      BOOST_CHECK(msg.getMessage() == "command");
      c.sendMessage(OK, msg.getEndpoint());
      msg = c.receiveMessage(boost::posix_time::seconds(1));
      BOOST_CHECK(msg.getMessage() == "takeoff");
      c.sendMessage(OK, msg.getEndpoint());
      msg = c.receiveMessage(boost::posix_time::seconds(1));
      BOOST_CHECK(msg.getMessage() == "up 200");
      c.sendMessage(OK, msg.getEndpoint());
      msg = c.receiveMessage(boost::posix_time::seconds(1));
      BOOST_CHECK(msg.getMessage() == "right 30");
      c.sendMessage(OK, msg.getEndpoint());
      msg = c.receiveMessage(boost::posix_time::seconds(1));
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
  GlobalRegistry::setRegistry(100, 20);
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
