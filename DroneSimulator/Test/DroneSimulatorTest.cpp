#include <boost/test/unit_test.hpp>

#include <boost/date_time/posix_time/posix_time.hpp>

#include "DroneMessagesLib/Message.hpp"
#include "DroneMessagesLib/Messages/BackMessage.hpp"
#include "DroneMessagesLib/Messages/CommandMessage.hpp"
#include "DroneMessagesLib/Messages/TakeoffMessage.hpp"
#include "UDPLib/Response.hpp"
#include "RegistryLib/Registry.hpp"
#include "UDPLib/UDPCommunicator.hpp"

#include "DroneSimulatorLib/DroneSimulator.hpp"
#include "DroneSimulatorLib/State/DroneSimulatorStateImpl.hpp"

namespace
{
  void startSimulator(boost::posix_time::seconds duration)
  {
    drone::DroneSimulator simulator(duration);
  }
}

BOOST_AUTO_TEST_CASE(DRONE_SIMULATOR_TEST)
{
  std::thread t1(startSimulator, boost::posix_time::seconds(1));
  auto command = std::make_shared<messages::CommandMessage>();
  auto takeoff = std::make_shared<messages::TakeoffMessage>();
  auto back = std::make_shared<messages::BackMessage>(20);
  auto pState = std::make_shared<drone::DroneSimulatorStateImpl>();
  udp::UDPCommunicator simulatorEndpoint;
  udp::UDPCommunicator statusEndpoint(8890);
  boost::asio::ip::udp::endpoint simulatorLocation(
    boost::asio::ip::address::from_string("127.0.0.1"), 8889);

  simulatorEndpoint.sendMessage(command->toString(), simulatorLocation);
  auto response =
    simulatorEndpoint.receiveMessage(boost::posix_time::seconds(1));
  BOOST_CHECK(response.didSucceed());
  BOOST_CHECK(response.getMessage() == "ok");

  simulatorEndpoint.sendMessage(takeoff->toString(), simulatorLocation);
  response = simulatorEndpoint.receiveMessage(boost::posix_time::seconds(1));
  BOOST_CHECK(response.didSucceed());
  BOOST_CHECK(response.getMessage() == "ok");

  simulatorEndpoint.sendMessage(back->toString(), simulatorLocation);
  response = simulatorEndpoint.receiveMessage(boost::posix_time::seconds(1));
  BOOST_CHECK(response.didSucceed());
  BOOST_CHECK(response.getMessage() == "ok");

  t1.join();
}
