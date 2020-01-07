#include <boost/test/unit_test.hpp>

#include "DroneMessagesLib/Messages/CommandMessage.hpp"
#include "DroneMessagesLib/MessageFactory.hpp"

namespace
{
  messages::CommandMessage goodMessage()
  {
    messages::CommandMessage commandMessage;
    std::string toParse("command");
    commandMessage.fromString(toParse);
    return commandMessage;
  }
}

BOOST_AUTO_TEST_CASE(COMMAND_MESSAGES_FROM_STRING_TEST)
{
  {
    messages::CommandMessage commandMessage;
    std::string toParse("commands");
    BOOST_CHECK_THROW(commandMessage.fromString(toParse), std::exception);
  }
  {
    messages::CommandMessage commandMessage;
    std::string toParse("command");
    BOOST_CHECK(commandMessage.fromString(toParse));
  }
}

BOOST_AUTO_TEST_CASE(COMMAND_MESSAGES_TO_STRING_TEST)
{
  auto message = goodMessage();
  BOOST_CHECK(message.toString() == "command");
}

BOOST_AUTO_TEST_CASE(MESSAGE_FACTORY_COMMAND_TEST)
{
  std::string testMessage = "command";

  BOOST_CHECK_EQUAL(
    boost::get<messages::CommandMessage>(messages::getMessage(testMessage))
    .toString(),
    testMessage);
}
