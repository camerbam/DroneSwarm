#include <boost/test/unit_test.hpp>

#include "DroneMessagesLib/MessageFactory.hpp"
#include "DroneMessagesLib/Messages/MonMessage.hpp"

namespace
{
  messages::MonMessage goodMessage()
  {
    messages::MonMessage monMessage;
    std::string toParse("mon");
    monMessage.fromString(toParse);
    return monMessage;
  }
}

BOOST_AUTO_TEST_CASE(MON_MESSAGES_FROM_STRING_TEST)
{
  {
    messages::MonMessage monMessage;
    std::string toParse("mons");
    BOOST_CHECK_THROW(monMessage.fromString(toParse), std::exception);
  }
  {
    messages::MonMessage monMessage;
    std::string toParse("mon");
    BOOST_CHECK(monMessage.fromString(toParse));
  }
}

BOOST_AUTO_TEST_CASE(MON_MESSAGES_TO_STRING_TEST)
{
  auto message = goodMessage();
  BOOST_CHECK(message.toString() == "mon");
}

BOOST_AUTO_TEST_CASE(MESSAGE_FACTORY_MON_TEST)
{
  std::string testMessage = "mon";

  BOOST_CHECK_EQUAL(
    boost::get<messages::MonMessage>(messages::getMessage(testMessage))
    .toString(),
    testMessage);
}
