#include <boost/test/auto_unit_test.hpp>

#include "DroneSimulatorLib/State/DroneSimulatorIdleState.hpp"
#include "UDPLib/UDPCommunicator.hpp"

BOOST_AUTO_TEST_CASE(DroneSimulatorIdleStateTest)
{
  udp::UDPCommunicator sender;
  auto pIdleState = std::make_shared<drone::DroneSimulatorIdleState>(sender);
  boost::asio::ip::udp::endpoint remote(
    boost::asio::ip::address_v4::from_string("127.0.0.1"), 45654);
  udp::Response response{remote, "takeoff", true};
  auto newState = pIdleState->handleResponse(response);
  BOOST_CHECK(newState);
  response = udp::Response{remote, "command", true};
  BOOST_CHECK(pIdleState->handleResponse(response));
}

BOOST_AUTO_TEST_CASE(DroneSimulatorIdleStateNoResponseTest)
{
  udp::UDPCommunicator sender;
  auto pIdleState = std::make_shared<drone::DroneSimulatorIdleState>(sender);
  boost::asio::ip::udp::endpoint remote;
  udp::Response response{remote, "bad message", false};
  BOOST_CHECK(pIdleState->handleResponse(response));
}
