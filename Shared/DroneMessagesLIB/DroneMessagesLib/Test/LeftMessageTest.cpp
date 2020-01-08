#include <boost/test/unit_test.hpp>

#include "DroneMessagesLib/Messages/LeftMessage.hpp"
#include "DroneMessagesLib/MessageFactory.hpp"

namespace
{
  messages::LeftMessage goodMessage(double distance)
  {
    messages::LeftMessage leftMessage;
    std::string toParse("left " + std::to_string(distance));
    leftMessage.fromString(toParse);
    return leftMessage;
  }
}

BOOST_AUTO_TEST_CASE(LEFT_MESSAGES_CONSTRUCTOR_TEST)
{
  {
    messages::LeftMessage leftMessage(40);
    BOOST_CHECK_EQUAL(leftMessage.getArgument(), 40);
  }
  BOOST_CHECK_THROW(messages::LeftMessage leftMessage(10), std::runtime_error);
  BOOST_CHECK_THROW(messages::LeftMessage leftMessage(510), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(LEFT_MESSAGES_FROM_STRING_TEST)
{
  {
    messages::LeftMessage leftMessage;
    std::string toParse("left 20");
    BOOST_CHECK(leftMessage.fromString(toParse));
  }
  {
    messages::LeftMessage leftMessage;
    std::string toParse("left 1000");
    BOOST_CHECK_THROW(leftMessage.fromString(toParse), std::exception);
  }
  {
    messages::LeftMessage leftMessage;
    std::string toParse("lefts 100");
    BOOST_CHECK_THROW(leftMessage.fromString(toParse), std::exception);
  }
  {
    messages::LeftMessage leftMessage;
    std::string toParse("left x");
    BOOST_CHECK_THROW(leftMessage.fromString(toParse), std::exception);
  }
  {
    messages::LeftMessage leftMessage;
    std::string toParse("left");
    BOOST_CHECK_THROW(leftMessage.fromString(toParse), std::exception);
  }
}

BOOST_AUTO_TEST_CASE(LEFT_MESSAGES_TO_STRING_TEST)
{
  {
    auto message = goodMessage(50.001);
    BOOST_CHECK(message.toString() == "left 50");
  }
  {
    auto message = goodMessage(500);
    BOOST_CHECK(message.toString() == "left 500");
  }
}

BOOST_AUTO_TEST_CASE(MESSAGE_FACTORY_LEFT_TEST)
{
  std::string testMessage = "left 58";

  BOOST_CHECK_EQUAL(
    boost::get<messages::LeftMessage>(messages::getMessage(testMessage))
    .toString(),
    testMessage);
}
