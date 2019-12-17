#include <boost/test/unit_test.hpp>

#include "DroneMessagesLib/MessageFactory.hpp"

BOOST_AUTO_TEST_CASE(MESSAGE_FACTORY_TEST_STRING)
{
  std::string str = "ups";
  BOOST_CHECK_THROW(messages::getMessage(str), std::exception);
}
