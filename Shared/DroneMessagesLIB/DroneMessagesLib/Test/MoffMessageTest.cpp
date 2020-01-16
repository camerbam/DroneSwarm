#include <boost/test/unit_test.hpp>

#include "DroneMessagesLib/MessageFactory.hpp"
#include "DroneMessagesLib/Messages/MoffMessage.hpp"

namespace
{
  messages::MoffMessage goodMessage()
  {
    messages::MoffMessage moffMessage;
    std::string toParse("moff");
    moffMessage.fromString(toParse);
    return moffMessage;
  }
}

BOOST_AUTO_TEST_CASE(MOFF_MESSAGES_FROM_STRING_TEST)
{
  {
    messages::MoffMessage moffMessage;
    std::string toParse("moffs");
    BOOST_CHECK_THROW(moffMessage.fromString(toParse), std::exception);
  }
  {
    messages::MoffMessage moffMessage;
    std::string toParse("moff");
    BOOST_CHECK(moffMessage.fromString(toParse));
  }
}

BOOST_AUTO_TEST_CASE(MOFF_MESSAGES_TO_STRING_TEST)
{
  auto message = goodMessage();
  BOOST_CHECK(message.toString() == "moff");
}

BOOST_AUTO_TEST_CASE(MESSAGE_FACTORY_MOFF_TEST)
{
  std::string testMessage = "moff";

  BOOST_CHECK_EQUAL(
    boost::get<messages::MoffMessage>(messages::getMessage(testMessage))
    .toString(),
    testMessage);
}
