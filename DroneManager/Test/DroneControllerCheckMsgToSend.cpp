#include <boost/test/unit_test.hpp>

#include "DroneManagerLib/DroneControllerCheckMsgToSend.hpp"
#include "DroneMessagesLib/Message_t.hpp"
/*
namespace
{
  boost::optional<std::string> checkMsg(
    messages::Message_t message,
    std::shared_ptr<drone::DroneControllerState>& pState)
  {
    return boost::apply_visitor(
      drone::DroneControllerCheckMsgToSend(pState), message);
  }
}

BOOST_AUTO_TEST_CASE(DroneControllerCheckMsgToSendTest)
{
  auto pDroneState = std::make_shared<drone::DroneControllerState>();

  // Grounded
  BOOST_CHECK(checkMsg(messages::BackMessage(50), pDroneState));
  BOOST_CHECK(checkMsg(messages::ClockwiseMessage(50), pDroneState));
  BOOST_CHECK(checkMsg(messages::CounterClockwiseMessage(50), pDroneState));
  BOOST_CHECK(checkMsg(messages::DownMessage(50), pDroneState));
  BOOST_CHECK(checkMsg(messages::ForwardMessage(50), pDroneState));
  BOOST_CHECK(checkMsg(messages::LandMessage(), pDroneState));
  BOOST_CHECK(checkMsg(messages::LeftMessage(50), pDroneState));
  BOOST_CHECK(checkMsg(messages::RightMessage(50), pDroneState));
  BOOST_CHECK(checkMsg(messages::UpMessage(50), pDroneState));

  BOOST_CHECK(!checkMsg(messages::TakeoffMessage(), pDroneState));

  pDroneState->takeoff();
  BOOST_CHECK(checkMsg(messages::MDirectionMessage(2), pDroneState));
  pDroneState->setDetection(messages::DETECTION_DIRECTION::BOTH);
  BOOST_CHECK(checkMsg(messages::TakeoffMessage(), pDroneState));

  BOOST_CHECK(!checkMsg(messages::BackMessage(50), pDroneState));
  BOOST_CHECK(!checkMsg(messages::ClockwiseMessage(50), pDroneState));
  BOOST_CHECK(!checkMsg(messages::CounterClockwiseMessage(50), pDroneState));
  BOOST_CHECK(!checkMsg(messages::DownMessage(50), pDroneState));
  BOOST_CHECK(!checkMsg(messages::ForwardMessage(50), pDroneState));
  BOOST_CHECK(!checkMsg(messages::GoMessage(50, 53, 54, 55), pDroneState));
  BOOST_CHECK(!checkMsg(messages::LandMessage(), pDroneState));
  BOOST_CHECK(!checkMsg(messages::MDirectionMessage(1), pDroneState));
  BOOST_CHECK(!checkMsg(messages::RightMessage(50), pDroneState));
  BOOST_CHECK(!checkMsg(messages::UpMessage(50), pDroneState));
}

BOOST_AUTO_TEST_CASE(DroneControllerCheckMsgToSendTestNobattery)
{
  auto pDroneState = std::make_shared<drone::DroneControllerState>(4);
  BOOST_CHECK(checkMsg(messages::TakeoffMessage(), pDroneState));
}
*/