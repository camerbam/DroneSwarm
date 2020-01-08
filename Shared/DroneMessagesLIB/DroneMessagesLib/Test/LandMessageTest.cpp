#include <boost/test/unit_test.hpp>

#include "DroneMessagesLib/MessageFactory.hpp"
#include "DroneMessagesLib/Messages/LandMessage.hpp"

namespace
{
  messages::LandMessage goodMessage()
  {
    messages::LandMessage landMessage;
    std::string toParse("land");
    landMessage.fromString(toParse);
    return landMessage;
  }
}

BOOST_AUTO_TEST_CASE(LAND_MESSAGES_FROM_STRING_TEST)
{
  {
    messages::LandMessage landMessage;
    std::string toParse("lands");
    BOOST_CHECK_THROW(landMessage.fromString(toParse), std::exception);
  }
  {
    messages::LandMessage landMessage;
    std::string toParse("land");
    BOOST_CHECK(landMessage.fromString(toParse));
  }
}

BOOST_AUTO_TEST_CASE(LAND_MESSAGES_TO_STRING_TEST)
{
  auto message = goodMessage();
  BOOST_CHECK(message.toString() == "land");
}

BOOST_AUTO_TEST_CASE(MESSAGE_FACTORY_LAND_TEST)
{
  std::string testMessage = "land";

  BOOST_CHECK_EQUAL(
    boost::get<messages::LandMessage>(messages::getMessage(testMessage))
    .toString(),
    testMessage);
}
