#include <boost/test/unit_test.hpp>

#include "DroneMessagesLib/MessageFactory.hpp"
#include "DroneMessagesLib/Messages/TimeMessage.hpp"

namespace
{
  messages::TimeMessage goodMessage()
  {
    messages::TimeMessage backMessage;
    std::string toParse("time?");
    backMessage.fromString(toParse);
    return backMessage;
  }
}

BOOST_AUTO_TEST_CASE(TIME_MESSAGES_FROM_STRING_TEST)
{
  {
    messages::TimeMessage timeMessage;
    std::string toParse("time");
    BOOST_CHECK_THROW(timeMessage.fromString(toParse), std::exception);
  }
  {
    messages::TimeMessage timeMessage;
    std::string toParse("times");
    BOOST_CHECK_THROW(timeMessage.fromString(toParse), std::exception);
  }
  {
    messages::TimeMessage timeMessage;
    std::string toParse("time?");
    BOOST_CHECK(timeMessage.fromString(toParse));
  }
}

BOOST_AUTO_TEST_CASE(TIME_MESSAGES_TO_STRING_TEST)
{
  auto message = goodMessage();
  BOOST_CHECK(message.toString() == "time?");
}

BOOST_AUTO_TEST_CASE(MESSAGE_FACTORY_TIME_TEST)
{
  std::string testMessage = "time?";

  BOOST_CHECK_EQUAL(
    boost::get<messages::TimeMessage>(messages::getMessage(testMessage))
    .toString(),
    testMessage);
}
