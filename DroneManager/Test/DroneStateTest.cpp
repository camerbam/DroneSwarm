#include <boost/test/unit_test.hpp>
#include <iostream>

#include "UtilsLib/Utils.hpp"

#include "DroneManagerLib/DroneControllerState.hpp"
#include "DroneMessagesLib/DroneStatusMessage.hpp"
/*
BOOST_AUTO_TEST_CASE(DroneControllerStateTest)
{
  messages::DroneStatusMessage msg;
  drone::DroneControllerState droneState;
  droneState.takeoff();
  std::atomic<bool> reachedBattery(false);
  std::atomic<bool> reachedMid(false);

  auto connectionBattery =
    droneState.registerForBattery([&reachedBattery](size_t) { reachedBattery = true; });
  auto connectionMid =
    droneState.registerForBattery([&reachedMid](size_t) { reachedMid = true; });
  droneState.updateStatus(msg.toString(1, 5, 6, 200, 40, 100, 99, 5));

  BOOST_CHECK_EQUAL(droneState.getMid(), 1);
  BOOST_CHECK_EQUAL(droneState.getX(), 5);
  BOOST_CHECK_EQUAL(droneState.getY(), 6);
  BOOST_CHECK_EQUAL(droneState.getZ(), 200);
  BOOST_CHECK_EQUAL(droneState.getAngle(), 40);
  BOOST_CHECK_EQUAL(droneState.getTimeOfFlight(), 100);

  BOOST_CHECK_EQUAL(droneState.getBattery(), 99);
  BOOST_CHECK_EQUAL(droneState.getTime(), 5);
  BOOST_CHECK_EQUAL(droneState.isFlying(), true);

  droneState.changeSpeed(75);
  BOOST_CHECK_EQUAL(droneState.getSpeed(), 75);

  droneState.setDetection(messages::DETECTION_DIRECTION::BOTH);
  BOOST_CHECK(droneState.getDirection() == messages::DETECTION_DIRECTION::BOTH);

  droneState.land();

  droneState.updateStatus(msg.toString(2, 6, 7, 200, 40, 100, 99, 5));
  BOOST_CHECK_EQUAL(droneState.getMid(), 2);
  BOOST_CHECK_EQUAL(droneState.getX(), 11);
  BOOST_CHECK_EQUAL(droneState.getY(), 13);

  BOOST_CHECK(reachedBattery);
  BOOST_CHECK(reachedMid);
}
*/