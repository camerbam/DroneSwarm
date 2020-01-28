#include <boost/test/unit_test.hpp>

#include "DroneMessagesLib/DroneStatusMessage.hpp"

BOOST_AUTO_TEST_CASE(DroneStatusMessageTest)
{
  messages::DroneStatusMessage msg;
  auto status = msg.toString(1, 5, 6, 200, 40, 100, 99, 5);
  msg.fromString(status);
  BOOST_CHECK_EQUAL(msg.getMid(), 1);
  BOOST_CHECK_EQUAL(msg.getXCoordinate(), 5);
  BOOST_CHECK_EQUAL(msg.getYCoordinate(), 6);
  BOOST_CHECK_EQUAL(msg.getZCoordinate(), 200);
  BOOST_CHECK_EQUAL(msg.getAngle(), 40);
  BOOST_CHECK_EQUAL(msg.getTimeOfFlight(), 100);
  BOOST_CHECK_EQUAL(msg.getBattery(), 99);
  BOOST_CHECK_EQUAL(msg.getTime(), 5);
}

BOOST_AUTO_TEST_CASE(DroneStatusInvalidMessageTest)
{
  messages::DroneStatusMessage msg;
  auto status = msg.toString(1, 5, 6, 200, 40, 100, 99, 5);
  BOOST_CHECK_THROW(msg.fromString(status+"l"), std::exception);
}
