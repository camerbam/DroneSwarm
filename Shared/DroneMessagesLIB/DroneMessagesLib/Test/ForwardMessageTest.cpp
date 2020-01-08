#include <boost/test/unit_test.hpp>

#include "DroneMessagesLib/MessageFactory.hpp"
#include "DroneMessagesLib/Messages/ForwardMessage.hpp"

namespace
{
  messages::ForwardMessage goodMessage(double distance)
  {
    messages::ForwardMessage forwardMessage;
    std::string toParse("forward " + std::to_string(distance));
    forwardMessage.fromString(toParse);
    return forwardMessage;
  }
}

BOOST_AUTO_TEST_CASE(FORWARD_MESSAGES_FROM_STRING_TEST)
{
  {
    messages::ForwardMessage forwardMessage;
    std::string toParse("forward 20");
    BOOST_CHECK(forwardMessage.fromString(toParse));
  }
  {
    messages::ForwardMessage forwardMessage;
    std::string toParse("forward 1000");
    BOOST_CHECK_THROW(forwardMessage.fromString(toParse), std::exception);
  }
  {
    messages::ForwardMessage forwardMessage;
    std::string toParse("forwards 100");
    BOOST_CHECK_THROW(forwardMessage.fromString(toParse), std::exception);
  }
  {
    messages::ForwardMessage forwardMessage;
    std::string toParse("forward x");
    BOOST_CHECK_THROW(forwardMessage.fromString(toParse), std::exception);
  }
  {
    messages::ForwardMessage forwardMessage;
    std::string toParse("forward");
    BOOST_CHECK_THROW(forwardMessage.fromString(toParse), std::exception);
  }
}

BOOST_AUTO_TEST_CASE(FORWARD_MESSAGES_CONSTRUCTOR_TEST)
{
  {
    messages::ForwardMessage forwardMessage(40);
    BOOST_CHECK_EQUAL(forwardMessage.getArgument(), 40);
  }
  BOOST_CHECK_THROW(messages::ForwardMessage forwardMessage(10), std::runtime_error);
  BOOST_CHECK_THROW(messages::ForwardMessage forwardMessage(510), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(FORWARD_MESSAGES_TO_STRING_TEST)
{
  {
    auto message = goodMessage(50.001);
    BOOST_CHECK(message.toString() == "forward 50");
  }
  {
    auto message = goodMessage(500);
    BOOST_CHECK(message.toString() == "forward 500");
  }
}

BOOST_AUTO_TEST_CASE(MESSAGE_FACTORY_FORWARD_TEST)
{
  std::string testMessage = "forward 52";

  BOOST_CHECK_EQUAL(
    boost::get<messages::ForwardMessage>(messages::getMessage(testMessage))
    .toString(),
    testMessage);
}
