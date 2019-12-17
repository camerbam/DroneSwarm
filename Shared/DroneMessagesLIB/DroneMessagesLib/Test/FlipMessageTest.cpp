#include <boost/test/unit_test.hpp>

#include "DroneMessagesLib/Messages/FlipMessage.hpp"
#include "DroneMessagesLib/MessageFactory.hpp"

namespace
{
  messages::FlipMessage goodMessage(const std::string& direction)
  {
    messages::FlipMessage flipMessage;
    std::string toParse("flip " + direction);
    flipMessage.fromString(toParse);
    return flipMessage;
  }

  void flipMessageFromStringTest(std::string& toParse, bool succeed)
  {
    messages::FlipMessage flipMessage;
    BOOST_CHECK(flipMessage.fromString(toParse) == succeed);
  }
}

BOOST_AUTO_TEST_CASE(FLIP_MESSAGES_CONSTRUCTOR_TEST)
{
  {
    messages::FlipMessage flipMessage("f");
    BOOST_CHECK(flipMessage.getDirection() == messages::Direction::FORWARD);
  }
  BOOST_CHECK_THROW(messages::FlipMessage flipMessage("u"), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(FLIP_MESSAGES_FROM_STRING_TEST)
{
  std::string command("flip f");
  flipMessageFromStringTest(command, true);
  command = "flip r";
  flipMessageFromStringTest(command, true);
  command = "flip b";
  flipMessageFromStringTest(command, true);
  command = "flip l";
  flipMessageFromStringTest(command, true);
  command = "flip u";
  flipMessageFromStringTest(command, false);
  command = "flip";
  flipMessageFromStringTest(command, false);
}

BOOST_AUTO_TEST_CASE(FLIP_MESSAGES_TO_STRING_TEST)
{
  {
    auto message = goodMessage("f");
    BOOST_CHECK(message.toString() == "flip f");
  }
  {
    auto message = goodMessage("r");
    BOOST_CHECK(message.toString() == "flip r");
  }
  {
    auto message = goodMessage("l");
    BOOST_CHECK(message.toString() == "flip l");
  }
  {
    auto message = goodMessage("b");
    BOOST_CHECK(message.toString() == "flip b");
  }
}

BOOST_AUTO_TEST_CASE(MESSAGE_FACTORY_FLIP_TEST)
{
  std::string testMessage = "flip f";

  BOOST_CHECK_EQUAL(
    boost::get<messages::FlipMessage>(messages::getMessage(testMessage))
    .toString(),
    testMessage);
}
