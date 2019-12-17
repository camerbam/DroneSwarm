#include <boost/test/unit_test.hpp>

#include "DroneMessagesLib/MessageFactory.hpp"
#include "DroneMessagesLib/Messages/SpeedMessage.hpp"

namespace
{
  messages::SpeedMessage goodMessage()
  {
    messages::SpeedMessage backMessage;
    std::string toParse("speed?");
    backMessage.fromString(toParse);
    return backMessage;
  }
}

BOOST_AUTO_TEST_CASE(SPEED_MESSAGES_FROM_STRING_TEST)
{
  {
    messages::SpeedMessage speedMessage;
    std::string toParse("speed");
    BOOST_CHECK_THROW(speedMessage.fromString(toParse), std::exception);
  }
  {
    messages::SpeedMessage speedMessage;
    std::string toParse("speeds");
    BOOST_CHECK_THROW(speedMessage.fromString(toParse), std::exception);
  }
  {
    messages::SpeedMessage speedMessage;
    std::string toParse("speed?");
    BOOST_CHECK(speedMessage.fromString(toParse));
  }
}

BOOST_AUTO_TEST_CASE(SPEED_MESSAGES_TO_STRING_TEST)
{
  auto message = goodMessage();
  BOOST_CHECK(message.toString() == "speed?");
}

BOOST_AUTO_TEST_CASE(MESSAGE_FACTORY_SPEED_TEST)
{
  std::string testMessage = "speed?";

  BOOST_CHECK_EQUAL(
    boost::get<messages::SpeedMessage>(messages::getMessage(testMessage))
    .toString(),
    testMessage);
}
