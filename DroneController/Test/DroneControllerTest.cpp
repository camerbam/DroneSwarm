#include <boost/test/unit_test.hpp>

#include <iostream>

#include "DroneControllerLib/DroneController.hpp"
#include "DroneSimulatorLib/DroneSimulator.hpp"
#include "DroneMessagesLib/MessageFactory.hpp"
#include "DroneMessagesLib/Messages/BackMessage.hpp"
#include "DroneMessagesLib/Messages/CommandMessage.hpp"
#include "DroneMessagesLib/Messages/ForwardMessage.hpp"
#include "DroneMessagesLib/Messages/LandMessage.hpp"
#include "DroneMessagesLib/Messages/TakeoffMessage.hpp"

namespace
{
  void startSimulator(boost::posix_time::seconds duration, size_t startBattery)
  {
    drone::DroneSimulator simulator(duration, startBattery);
  }

  bool compareTwoDoubles(const double& a, const double& b)
  {
    return std::abs(a - b) < .25;
  }
}

BOOST_AUTO_TEST_CASE(DRONE_CONTROLLER_TEST)
{
  std::thread t1(startSimulator, boost::posix_time::seconds(5), 100);

  auto controller = std::make_shared<drone::DroneController>("127.0.0.1");

  messages::Message_t command = messages::CommandMessage();
  messages::Message_t takeoff = messages::TakeoffMessage();
  messages::Message_t back = messages::BackMessage(20);
  messages::Message_t land = messages::LandMessage();
  BOOST_CHECK(controller->sendMessage(command));
  BOOST_CHECK(controller->sendMessage(takeoff));
  BOOST_CHECK(controller->sendMessage(back));
  BOOST_CHECK(controller->sendMessage(land));
  std::this_thread::sleep_for(std::chrono::seconds(3));
  BOOST_CHECK(controller->getBattery() == 97);
  controller.reset();
  t1.join();
}

BOOST_AUTO_TEST_CASE(DRONE_SIMULATOR_TWO_CONTROLLER_TEST)
{
  std::thread t1(startSimulator, boost::posix_time::seconds(5), 100);
  std::this_thread::sleep_for(std::chrono::seconds(3));
  auto command = messages::CommandMessage();
  messages::Message_t takeoff = messages::TakeoffMessage();
  messages::Message_t back = messages::BackMessage(20);
  auto forward = messages::ForwardMessage(20);
  auto firstController = std::make_shared<drone::DroneController>("127.0.0.1");
  boost::asio::ip::udp::endpoint simulatorLocation(
    boost::asio::ip::address::from_string("127.0.0.1"), 8889);
  udp::UDPCommunicator simulatorEndpointFirst;

  simulatorEndpointFirst.sendMessage(command.toString(), simulatorLocation);
  firstController->sendMessage(takeoff);
  firstController->sendMessage(back);
  simulatorEndpointFirst.sendMessage(forward.toString(), simulatorLocation);

  BOOST_CHECK(compareTwoDoubles(firstController->getX(), -20));
  t1.join();
}

BOOST_AUTO_TEST_CASE(DRONE_CONTROLLER_LATE_SIMULATOR_TEST)
{
  auto controller = std::make_shared<drone::DroneController>("127.0.0.1");
  std::string toSend("command");
  BOOST_CHECK(controller->sendMessage(messages::getMessage(toSend)) ==
              boost::none);
  std::this_thread::sleep_for(std::chrono::seconds(2));
  std::thread t1(startSimulator, boost::posix_time::seconds(2), 100);
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  BOOST_CHECK(controller->sendMessage(messages::getMessage(toSend)) !=
              boost::none);
  controller.reset();
  t1.join();
}

BOOST_AUTO_TEST_CASE(DRONE_CONTROLLER_LOW_BATTERY_TEST)
{
  std::thread t1(startSimulator, boost::posix_time::seconds(2), 15);
  auto controller = std::make_shared<drone::DroneController>("127.0.0.1");

  std::this_thread::sleep_for(std::chrono::seconds(2));

  BOOST_CHECK(!controller->getIsRunning());
  t1.join();
}

BOOST_AUTO_TEST_CASE(DRONE_CONTROLLER_SUSTAINED_TEST)
{
  std::thread t1(startSimulator, boost::posix_time::seconds(2), 100);
  auto controller = std::make_shared<drone::DroneController>("127.0.0.1");

  std::this_thread::sleep_for(std::chrono::seconds(10));

  BOOST_CHECK(controller->getIsRunning());
  controller.reset();
  t1.join();
}

BOOST_AUTO_TEST_CASE(DRONE_CONTROLLER_UPDATE_STATE_TEST)
{
  std::thread t1(startSimulator, boost::posix_time::seconds(10), 100);
  auto controller = std::make_shared<drone::DroneController>("127.0.0.1");

  messages::Message_t takeoff = messages::TakeoffMessage();
  messages::Message_t back = messages::BackMessage(25);
  messages::Message_t left = messages::LeftMessage(20);
  messages::Message_t land = messages::LandMessage();

  BOOST_CHECK(controller->sendMessage(takeoff));
  BOOST_CHECK(controller->sendMessage(back));
  BOOST_CHECK(controller->sendMessage(left));
  BOOST_CHECK(controller->sendMessage(land));

  BOOST_CHECK(compareTwoDoubles(controller->getX(), -25));
  BOOST_CHECK(compareTwoDoubles(controller->getY(), -20));
  BOOST_CHECK(compareTwoDoubles(controller->getZ(), 0));

  std::this_thread::sleep_for(std::chrono::milliseconds(1500));

  BOOST_CHECK(controller->getSpeed() == 10);

  BOOST_CHECK(controller->getTime() == 6);
  BOOST_CHECK_EQUAL(controller->getTimeOfFlight(), 245);

  controller.reset();
  t1.join();
}
