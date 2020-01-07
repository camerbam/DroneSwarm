#include <boost/test/unit_test.hpp>

#include "DroneMessagesLib/MessageFactory.hpp"
#include "DroneMessagesLib/Messages/DownMessage.hpp"

namespace
{
  messages::DownMessage goodMessage(double distance)
  {
    messages::DownMessage downMessage;
    std::string toParse("down " + std::to_string(distance));
    downMessage.fromString(toParse);
    return downMessage;
  }
}

BOOST_AUTO_TEST_CASE(DOWN_MESSAGES_CONSTRUCTOR_TEST)
{
  {
    messages::DownMessage downMessage(40);
    BOOST_CHECK_EQUAL(downMessage.getArgument(), 40);
  }
  BOOST_CHECK_THROW(messages::DownMessage downMessage(10), std::runtime_error);
  BOOST_CHECK_THROW(messages::DownMessage downMessage(510), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(DOWN_MESSAGES_FROM_STRING_TEST)
{
  {
    messages::DownMessage downMessage;
    std::string toParse("down 20");
    BOOST_CHECK(downMessage.fromString(toParse));
  }
  {
    messages::DownMessage downMessage;
    std::string toParse("down 1000");
    BOOST_CHECK_THROW(downMessage.fromString(toParse), std::exception);
  }
  {
    messages::DownMessage downMessage;
    std::string toParse("downs 100");
    BOOST_CHECK_THROW(downMessage.fromString(toParse), std::exception);
  }
  {
    messages::DownMessage downMessage;
    std::string toParse("down x");
    BOOST_CHECK_THROW(downMessage.fromString(toParse), std::exception);
  }
  {
    messages::DownMessage downMessage;
    std::string toParse("down");
    BOOST_CHECK_THROW(downMessage.fromString(toParse), std::exception);
  }
}

BOOST_AUTO_TEST_CASE(DOWN_MESSAGES_TO_STRING_TEST)
{
  {
    auto message = goodMessage(50.001);
    BOOST_CHECK(message.toString() == "down 50");
  }
  {
    auto message = goodMessage(500);
    BOOST_CHECK(message.toString() == "down 500");
  }
}

BOOST_AUTO_TEST_CASE(MESSAGE_FACTORY_DOWN_TEST)
{
  std::string testMessage = "down 45";

  BOOST_CHECK_EQUAL(
    boost::get<messages::DownMessage>(messages::getMessage(testMessage))
    .toString(),
    testMessage);
}
