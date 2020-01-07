#include <boost/test/unit_test.hpp>

#include "DroneMessagesLib/MessageFactory.hpp"
#include "DroneMessagesLib/Messages/TakeoffMessage.hpp"

namespace
{
  messages::TakeoffMessage goodMessage()
  {
    messages::TakeoffMessage takeoffMessage;
    std::string toParse("takeoff");
    takeoffMessage.fromString(toParse);
    return takeoffMessage;
  }
}

BOOST_AUTO_TEST_CASE(TAKEOFF_MESSAGES_FROM_STRING_TEST)
{
  {
    messages::TakeoffMessage takeoffMessage;
    std::string toParse("takeoffs");
    BOOST_CHECK_THROW(takeoffMessage.fromString(toParse), std::exception);
  }
  {
    messages::TakeoffMessage takeoffMessage;
    std::string toParse("takeoff");
    BOOST_CHECK(takeoffMessage.fromString(toParse));
  }
}

BOOST_AUTO_TEST_CASE(TAKEOFF_MESSAGES_TO_STRING_TEST)
{
  auto message = goodMessage();
  BOOST_CHECK(message.toString() == "takeoff");
}

BOOST_AUTO_TEST_CASE(MESSAGE_FACTORY_TAKEOFF_TEST)
{
  std::string testMessage = "takeoff";

  BOOST_CHECK_EQUAL(
    boost::get<messages::TakeoffMessage>(messages::getMessage(testMessage))
    .toString(),
    testMessage);
}
