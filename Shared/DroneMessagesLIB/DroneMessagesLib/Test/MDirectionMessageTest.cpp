#include <boost/test/unit_test.hpp>

#include "DroneMessagesLib/MessageFactory.hpp"
#include "DroneMessagesLib/Messages/MDirectionMessage.hpp"

namespace
{
  messages::MDirectionMessage goodMessage(size_t direction)
  {
    messages::MDirectionMessage mDirectionMessage;
    std::string toParse("mdirection " + std::to_string(direction));
    mDirectionMessage.fromString(toParse);
    return mDirectionMessage;
  }
}

BOOST_AUTO_TEST_CASE(MDIRECTION_MESSAGES_CONSTRUCTOR_TEST)
{
  {
    messages::MDirectionMessage mDirectionMessage(2);
    BOOST_CHECK_EQUAL(mDirectionMessage.getArgument(), 2);
  }
  BOOST_CHECK_THROW(
    messages::MDirectionMessage mDirectionMessage(10), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(MDIRECTION_MESSAGES_FROM_STRING_TEST)
{
  {
    messages::MDirectionMessage mDirectionMessage;
    std::string toParse("mdirection 1");
    BOOST_CHECK(mDirectionMessage.fromString(toParse));
  }
  {
    messages::MDirectionMessage mDirectionMessage;
    std::string toParse("mdirection 1000");
    BOOST_CHECK_THROW(mDirectionMessage.fromString(toParse), std::exception);
  }
}

BOOST_AUTO_TEST_CASE(MDIRECTION_MESSAGES_TO_STRING_TEST)
{
  {
    auto message = goodMessage(1);
    BOOST_CHECK(message.toString() == "mdirection 1");
  }
}

BOOST_AUTO_TEST_CASE(MESSAGE_FACTORY_MDIRECTION_TEST)
{
  std::string testMessage = "mdirection 2";

  BOOST_CHECK_EQUAL(
    boost::get<messages::MDirectionMessage>(messages::getMessage(testMessage))
      .toString(),
    testMessage);
}
