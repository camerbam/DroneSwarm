#include <boost/test/unit_test.hpp>

#include "DroneMessagesLib/MessageFactory.hpp"
#include "DroneMessagesLib/Messages/BatteryMessage.hpp"

namespace
{
  messages::BatteryMessage goodMessage()
  {
    messages::BatteryMessage backMessage;
    std::string toParse("battery?");
    backMessage.fromString(toParse);
    return backMessage;
  }
}

BOOST_AUTO_TEST_CASE(BATTERY_MESSAGES_FROM_STRING_TEST)
{
  {
    messages::BatteryMessage batteryMessage;
    std::string toParse("battery");
    BOOST_CHECK_THROW(batteryMessage.fromString(toParse), std::exception);
  }
  {
    messages::BatteryMessage batteryMessage;
    std::string toParse("batterys");
    BOOST_CHECK_THROW(batteryMessage.fromString(toParse), std::exception);
  }
  {
    messages::BatteryMessage batteryMessage;
    std::string toParse("battery?");
    BOOST_CHECK(batteryMessage.fromString(toParse));
  }
}

BOOST_AUTO_TEST_CASE(BATTERY_MESSAGES_TO_STRING_TEST)
{
  auto message = goodMessage();
  BOOST_CHECK(message.toString() == "battery?");
}

BOOST_AUTO_TEST_CASE(MESSAGE_FACTORY_BATTERY_TEST)
{
  std::string testMessage = "battery?";

  BOOST_CHECK_EQUAL(
    boost::get<messages::BatteryMessage>(messages::getMessage(testMessage))
    .toString(),
    testMessage);
}
