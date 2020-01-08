#include <boost/test/unit_test.hpp>

#include "DroneMessagesLib/MessageFactory.hpp"
#include "DroneMessagesLib/Messages/UpMessage.hpp"

namespace
{
  messages::UpMessage goodMessage(double distance)
  {
    messages::UpMessage upMessage;
    std::string toParse("up " + std::to_string(distance));
    upMessage.fromString(toParse);
    return upMessage;
  }
}

BOOST_AUTO_TEST_CASE(UP_MESSAGES_CONSTRUCTOR_TEST)
{
  {
    messages::UpMessage upMessage(40);
    BOOST_CHECK_EQUAL(upMessage.getArgument(), 40);
  }
  BOOST_CHECK_THROW(messages::UpMessage upMessage(10), std::runtime_error);
  BOOST_CHECK_THROW(messages::UpMessage upMessage(510), std::runtime_error);
}


BOOST_AUTO_TEST_CASE(UP_MESSAGES_FROM_STRING_TEST)
{
  {
    messages::UpMessage upMessage;
    std::string toParse("up 20");
    BOOST_CHECK(upMessage.fromString(toParse));
  }
  {
    messages::UpMessage upMessage;
    std::string toParse("up 1000");
    BOOST_CHECK_THROW(upMessage.fromString(toParse), std::exception);
  }
  {
    messages::UpMessage upMessage;
    std::string toParse("ups 100");
    BOOST_CHECK_THROW(upMessage.fromString(toParse), std::exception);
  }
  {
    messages::UpMessage upMessage;
    std::string toParse("up x");
    BOOST_CHECK_THROW(upMessage.fromString(toParse), std::exception);
  }
  {
    messages::UpMessage upMessage;
    std::string toParse("up");
    BOOST_CHECK_THROW(upMessage.fromString(toParse), std::exception);
  }
}

BOOST_AUTO_TEST_CASE(UP_MESSAGES_TO_STRING_TEST)
{
  {
    auto message = goodMessage(50.001);
    BOOST_CHECK(message.toString() == "up 50");
  }
  {
    auto message = goodMessage(500);
    BOOST_CHECK(message.toString() == "up 500");
  }
}

BOOST_AUTO_TEST_CASE(MESSAGE_FACTORY_UP_TEST)
{
  std::string testMessage = "up 61";

  BOOST_CHECK_EQUAL(
    boost::get<messages::UpMessage>(messages::getMessage(testMessage))
    .toString(),
    testMessage);
}
