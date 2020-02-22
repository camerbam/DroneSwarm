#include <boost/test/unit_test.hpp>

#include <iostream>

#include "DroneManagerLib/DroneController.hpp"
#include "DroneMessagesLib/MessageFactory.hpp"
#include "DroneMessagesLib/Messages/BackMessage.hpp"
#include "DroneMessagesLib/Messages/CommandMessage.hpp"
#include "DroneMessagesLib/Messages/ForwardMessage.hpp"
#include "DroneMessagesLib/Messages/LandMessage.hpp"
#include "DroneMessagesLib/Messages/TakeoffMessage.hpp"
#include "DroneSimulatorLib/DroneSimulator.hpp"
#include "MonitorLoggerLib/MonitorLogger.hpp"
#include "RegistryLib/Registry.hpp"
#include "UtilsLib/Utils.hpp"

namespace
{
  void startSimulator(boost::posix_time::seconds duration, size_t startBattery)
  {
    drone::DroneSimulator simulator(8889, duration, startBattery);
  }
}

BOOST_AUTO_TEST_CASE(DRONE_CONTROLLER_TEST)
{
  GlobalRegistry::setRegistry(100, 20, {{30, 40, 2}}, true, false);

  std::thread t1(startSimulator, boost::posix_time::seconds(1), 100);

  logger::MonitorLogger logger("Drone Manager", "localhost", "13000");

  auto controller =
    std::make_shared<drone::DroneController>(logger, 8889, 8890, "127.0.0.1");

  messages::Message_t command = messages::CommandMessage();
  messages::Message_t takeoff = messages::TakeoffMessage();
  messages::Message_t back = messages::BackMessage(20);
  messages::Message_t land = messages::LandMessage();
  BOOST_CHECK(controller->sendMessage(command));
  BOOST_CHECK(controller->sendMessage(takeoff));
  BOOST_CHECK(controller->sendMessage(back));
  BOOST_CHECK(controller->sendMessage(land));
  BOOST_CHECK(
    utils::checkWithin(static_cast<int>(controller->getBattery()), 97, 2));
  controller.reset();
  t1.join();
}

// BOOST_AUTO_TEST_CASE(DRONE_SIMULATOR_TWO_CONTROLLER_TEST) // TODO Move to
// Simulator
//{
//  std::thread t1(startSimulator, boost::posix_time::seconds(1), 100);
//  auto command = messages::CommandMessage();
//  messages::Message_t takeoff = messages::TakeoffMessage();
//  messages::Message_t back = messages::BackMessage(20);
//  auto forward = messages::ForwardMessage(20);
//  auto firstController =
//  std::make_shared<drone::DroneController>("127.0.0.1");
//  boost::asio::ip::udp::endpoint simulatorLocation(
//    boost::asio::ip::address::from_string("127.0.0.1"), 8889);
//  udp::UDPCommunicator simulatorEndpointFirst;
//
//  simulatorEndpointFirst.sendMessage(command.toString(), simulatorLocation);
//  firstController->sendMessage(takeoff);
//  firstController->sendMessage(back);
//  simulatorEndpointFirst.sendMessage(forward.toString(), simulatorLocation);
//
//  BOOST_CHECK(utils::compareTwoDoubles(firstController->getX(), -20));
//  t1.join();
//}

// BOOST_AUTO_TEST_CASE(DRONE_CONTROLLER_LATE_SIMULATOR_TEST)
//{
//  auto controller = std::make_shared<drone::DroneController>("127.0.0.1");
//  std::string toSend("command");
//  BOOST_CHECK(controller->sendMessage(messages::getMessage(toSend),
//  boost::posix_time::seconds(1)) ==
//              boost::none);
//  std::thread t1(startSimulator, boost::posix_time::seconds(1), 100);
//  std::this_thread::sleep_for(std::chrono::microseconds(100));
//  BOOST_CHECK(controller->sendMessage(messages::getMessage(toSend)) !=
//              boost::none);
//  t1.join();
//}

BOOST_AUTO_TEST_CASE(DRONE_CONTROLLER_UPDATE_STATE_TEST)
{
  std::thread t1(startSimulator, boost::posix_time::seconds(1), 100);
  logger::MonitorLogger logger("Drone Manager", "localhost", "13000");

  auto controller =
    std::make_shared<drone::DroneController>(logger, 8889, 8890, "127.0.0.1");

  messages::Message_t takeoff = messages::TakeoffMessage();
  messages::Message_t back = messages::BackMessage(25);
  messages::Message_t left = messages::LeftMessage(20);
  messages::Message_t land = messages::LandMessage();

  BOOST_CHECK(controller->sendMessage(takeoff));
  BOOST_CHECK(controller->sendMessage(back));
  BOOST_CHECK(controller->sendMessage(left));
  BOOST_CHECK(controller->sendMessage(land));

  std::this_thread::sleep_for(std::chrono::milliseconds(510));
  BOOST_CHECK(utils::compareTwoDoubles(controller->getX(), -25));
  BOOST_CHECK(utils::compareTwoDoubles(controller->getY(), -20));
  BOOST_CHECK(utils::compareTwoDoubles(controller->getZ(), 0));

  BOOST_CHECK(controller->getSpeed() == 10);

  BOOST_CHECK(
    utils::checkWithin(static_cast<int>(controller->getTime()), 90, 10));
  BOOST_CHECK(utils::checkWithin(
    static_cast<int>(controller->getTimeOfFlight()), 245, 5));

  t1.join();
}
