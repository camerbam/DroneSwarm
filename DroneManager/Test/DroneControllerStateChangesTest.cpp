#include <boost/test/unit_test.hpp>
#include <iostream>

#include "DroneManagerLib/DroneControllerStateChanges.hpp"
#include "DroneMessagesLib/MessageFactory.hpp"
#include "DroneMessagesLib/Message_t.hpp"
#include "RegistryLib/Registry.hpp"
#include "UtilsLib/Utils.hpp"
/*
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
  BOOST_CHECK(pDroneState->getDirection() == messages::DETECTION_DIRECTION::NONE);

  checkMsg("takeoff", pDroneState);
  BOOST_CHECK_EQUAL(pDroneState->isFlying(), true);
  checkMsg("go 55 56 57 58", pDroneState);
  BOOST_CHECK_EQUAL(pDroneState->getSpeed(), 58);
  checkMsg("mon", pDroneState);
  BOOST_CHECK(pDroneState->getDirection() == messages::DETECTION_DIRECTION::BOTH);
  checkMsg("mdirection 1", pDroneState);
  BOOST_CHECK(pDroneState->getDirection() == messages::DETECTION_DIRECTION::DOWN);
  checkMsg("moff", pDroneState);
  BOOST_CHECK(pDroneState->getDirection() == messages::DETECTION_DIRECTION::NONE);
  checkMsg("land", pDroneState);
  BOOST_CHECK_EQUAL(pDroneState->isFlying(), false);
}
*/