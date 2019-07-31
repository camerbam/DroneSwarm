#include <boost/test/unit_test.hpp>

#include "MsgLib/StringMsg.hpp"

void testStringMsg(const msg::FORMAT& format)
{
  msg::StringMsg stringMsg;
  stringMsg.msg("test");

  msg::StringMsg answerMsg;
  auto msgAsString = msg::toString(stringMsg, format);
  msg::parseString(answerMsg, msgAsString, format);
  BOOST_CHECK(stringMsg.msg() == answerMsg.msg());
}

BOOST_AUTO_TEST_CASE(StringMsgTest)
{
  testStringMsg(msg::FORMAT::JSON);
  testStringMsg(msg::FORMAT::PROTOBUF);
  testStringMsg(msg::FORMAT::XML);
}
