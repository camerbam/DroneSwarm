#include <boost/test/unit_test.hpp>

#include "DroneMessagesLib/Messages/BackMessage.hpp"
#include "DroneMessagesLib/MessageFactory.hpp"

namespace
{
  messages::BackMessage goodMessage(double distance)
  {
    messages::BackMessage backMessage;
    std::string toParse("back " + std::to_string(distance));
    backMessage.fromString(toParse);
    return backMessage;
  }
}

BOOST_AUTO_TEST_CASE(BACK_MESSAGES_CONSTRUCTOR_TEST)
{
  {
    messages::BackMessage backMessage(40);
    BOOST_CHECK_EQUAL(backMessage.getArgument(), 40);
  }
  BOOST_CHECK_THROW(messages::BackMessage backMessage(10), std::runtime_error);
  BOOST_CHECK_THROW(messages::BackMessage backMessage(510), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(BACK_MESSAGES_FROM_STRING_TEST)
{
  {
    messages::BackMessage backMessage;
    std::string toParse("back 20");
    BOOST_CHECK(backMessage.fromString(toParse));
  }
  {
    messages::BackMessage backMessage;
    std::string toParse("back 1000");
    BOOST_CHECK_THROW(backMessage.fromString(toParse), std::exception);
  }
  {
    messages::BackMessage backMessage;
    std::string toParse("backs 100");
    BOOST_CHECK_THROW(backMessage.fromString(toParse), std::exception);
  }
  {
    messages::BackMessage backMessage;
    std::string toParse("back x");
    BOOST_CHECK_THROW(backMessage.fromString(toParse), std::exception);
  }
  {
    messages::BackMessage backMessage;
    std::string toParse("back");
    BOOST_CHECK_THROW(backMessage.fromString(toParse), std::exception);
  }
}

BOOST_AUTO_TEST_CASE(BACK_MESSAGES_TO_STRING_TEST)
{
  {
    auto message = goodMessage(50.001);
    BOOST_CHECK(message.toString() == "back 50");
  }
  {
    auto message = goodMessage(500);
    BOOST_CHECK(message.toString() == "back 500");
  }
}

BOOST_AUTO_TEST_CASE(MESSAGE_FACTORY_BACK_TEST)
{
  std::string testMessage = "back 50";

  BOOST_CHECK_EQUAL(
    boost::get<messages::BackMessage>(messages::getMessage(testMessage))
    .toString(),
    testMessage);
}
