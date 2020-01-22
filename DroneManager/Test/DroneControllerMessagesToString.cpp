#include <boost/test/unit_test.hpp>
#include <iostream>

#include "DroneManagerLib/DroneControllerMessagesToString.hpp"
#include "DroneMessagesLib/MessageFactory.hpp"
#include "DroneMessagesLib/Message_t.hpp"
/*
namespace
{
  void checkMsg(std::string message,
                std::shared_ptr<drone::DroneControllerState>& pState,
                const std::string& answer)
  {
    auto response =
      boost::apply_visitor(drone::DroneControllerMessagesToString(pState),
                           messages::getMessage(message));
    BOOST_CHECK(response == answer);
  }
} // namespace

BOOST_AUTO_TEST_CASE(DroneControllerMessagesToStringTest)
{
  auto pDroneState = std::make_shared<drone::DroneControllerState>();

  checkMsg("takeoff", pDroneState, "takeoff");
  checkMsg("back 50", pDroneState, "back 50");
  checkMsg("battery?", pDroneState, "battery?");
  checkMsg("cw 50", pDroneState, "cw 50");
  checkMsg("command", pDroneState, "command");
  checkMsg("ccw 50", pDroneState, "ccw 50");
  checkMsg("down 50", pDroneState, "down 50");
  checkMsg("flip f", pDroneState, "flip f");
  checkMsg("flip r", pDroneState, "flip r");
  checkMsg("flip l", pDroneState, "flip l");
  checkMsg("flip b", pDroneState, "flip b");
  checkMsg("forward 50", pDroneState, "forward 50");
  checkMsg("left 50", pDroneState, "left 50");
  checkMsg("right 50", pDroneState, "right 50");
  checkMsg("speed?", pDroneState, "speed?");
  checkMsg("go 50 50 50 50", pDroneState, "go 50 50 50 50");
  checkMsg("time?", pDroneState, "time?");
  checkMsg("up 50", pDroneState, "up 50");
  checkMsg("land", pDroneState, "land");
  checkMsg("mon", pDroneState, "mon");
  checkMsg("mdirection 0", pDroneState, "mdirection 0");
  checkMsg("moff", pDroneState, "moff");

  std::string status =
    "mid:1;x:5;y:6;z:100;mpry:0,0,0;pitch:0;roll:0;yaw:0;vgx:0;vgy:0;vgz:0;"
    "templ:85;temph:87;tof:100;h:100;bat:35;baro:1263.39;time:100;agx:3.00;agy:"
    "-4.00;agz:-1008.00;";
  pDroneState->updateStatus(status);

  checkMsg("flip f", pDroneState, "forward 20");
  checkMsg("flip r", pDroneState, "right 20");
  checkMsg("flip l", pDroneState, "left 20");
  checkMsg("flip b", pDroneState, "back 20");

  BOOST_CHECK_EQUAL(pDroneState->getX(), 5);
  BOOST_CHECK_EQUAL(pDroneState->getY(), 6);  
  BOOST_CHECK_EQUAL(pDroneState->getZ(), 100);
  BOOST_CHECK_EQUAL(pDroneState->getMid(), 1);
}
*/