#include <boost/test/unit_test.hpp>

#include "DroneControllerLib/DroneControllerStateChanges.hpp"
#include "DroneMessagesLib/MessageFactory.hpp"
#include "DroneMessagesLib/Message_t.hpp"
#include "UtilsLib/Utils.hpp"

namespace
{
  void checkMsg(std::string message,
                std::shared_ptr<drone::DroneControllerState>& pState)
  {
    boost::apply_visitor(drone::DroneControllerStateChanges(pState),
                         messages::getMessage(message));
  }
}

BOOST_AUTO_TEST_CASE(DroneControllerStateChangesTest)
{
  auto pDroneState = std::make_shared<drone::DroneControllerState>();

  checkMsg("takeoff", pDroneState);
  checkMsg("back 50", pDroneState);
  checkMsg("battery?", pDroneState);
  checkMsg("cw 150", pDroneState);
  checkMsg("command", pDroneState);
  checkMsg("ccw 50", pDroneState);
  checkMsg("down 51", pDroneState);
  checkMsg("flip f", pDroneState);
  checkMsg("flip r", pDroneState);
  checkMsg("flip l", pDroneState);
  checkMsg("flip b", pDroneState);
  checkMsg("forward 52", pDroneState);
  checkMsg("left 53", pDroneState);
  checkMsg("right 54", pDroneState);
  checkMsg("speed?", pDroneState);
  checkMsg("go 55 56 57 58", pDroneState);
  checkMsg("time?", pDroneState);
  checkMsg("up 59", pDroneState);
  checkMsg("land", pDroneState);

  BOOST_CHECK(utils::compareTwoDoubles(pDroneState->getAngle(), 260));
  BOOST_CHECK_EQUAL(pDroneState->getBattery(), 100);
  BOOST_CHECK_EQUAL(pDroneState->getSpeed(), 58);
  BOOST_CHECK_EQUAL(pDroneState->getTime(), 0);
  BOOST_CHECK(utils::compareTwoDoubles(pDroneState->getTimeOfFlight(), 0));
  BOOST_CHECK(utils::compareTwoDoubles(pDroneState->getX(), 57));
  BOOST_CHECK(utils::compareTwoDoubles(pDroneState->getY(), 57));
  BOOST_CHECK(utils::compareTwoDoubles(pDroneState->getZ(), 0));
}
