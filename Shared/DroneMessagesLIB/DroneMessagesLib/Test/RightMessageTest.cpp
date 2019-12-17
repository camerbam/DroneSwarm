#include <boost/test/unit_test.hpp>

#include "DroneMessagesLib/Messages/RightMessage.hpp"
#include "DroneMessagesLib/MessageFactory.hpp"

namespace
{
  messages::RightMessage goodMessage(double distance)
  {
    messages::RightMessage rightMessage;
    std::string toParse("right " + std::to_string(distance));
    rightMessage.fromString(toParse);
    return rightMessage;
  }
}

BOOST_AUTO_TEST_CASE(RIGHT_MESSAGES_CONSTRUCTOR_TEST)
{
  {
    messages::RightMessage rightMessage(40);
    BOOST_CHECK_EQUAL(rightMessage.getArgument(), 40);
  }
  BOOST_CHECK_THROW(messages::RightMessage rightMessage(10), std::runtime_error);
  BOOST_CHECK_THROW(messages::RightMessage rightMessage(510), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(RIGHT_MESSAGES_FROM_STRING_TEST)
{
  {
    messages::RightMessage rightMessage;
    std::string toParse("right 20");
    BOOST_CHECK(rightMessage.fromString(toParse));
  }
  {
    messages::RightMessage rightMessage;
    std::string toParse("right 1000");
    BOOST_CHECK_THROW(rightMessage.fromString(toParse), std::exception);
  }
  {
    messages::RightMessage rightMessage;
    std::string toParse("rights 100");
    BOOST_CHECK_THROW(rightMessage.fromString(toParse), std::exception);
  }
  {
    messages::RightMessage rightMessage;
    std::string toParse("right x");
    BOOST_CHECK_THROW(rightMessage.fromString(toParse), std::exception);
  }
  {
    messages::RightMessage rightMessage;
    std::string toParse("right");
    BOOST_CHECK_THROW(rightMessage.fromString(toParse), std::exception);
  }
}

BOOST_AUTO_TEST_CASE(RIGHT_MESSAGES_TO_STRING_TEST)
{
  {
    auto message = goodMessage(50.001);
    BOOST_CHECK(message.toString() == "right 50");
  }
  {
    auto message = goodMessage(500);
    BOOST_CHECK(message.toString() == "right 500");
  }
}

BOOST_AUTO_TEST_CASE(MESSAGE_FACTORY_RIGHT_TEST)
{
  std::string testMessage = "right 59";

  BOOST_CHECK_EQUAL(
    boost::get<messages::RightMessage>(messages::getMessage(testMessage))
    .toString(),
    testMessage);
}
