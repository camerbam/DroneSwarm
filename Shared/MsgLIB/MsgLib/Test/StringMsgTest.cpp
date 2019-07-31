#include <boost/test/unit_test.hpp>

#include "MsgLib/StringMsg.hpp"

void testStringMsg(const msg::FORMAT& format)
{
  msg::StringMsg stringMsg;
  stringMsg.msg("test");
  stringMsg.format(format);

  msg::StringMsg answerMsg;
  auto msgAsString = stringMsg.toString();
  answerMsg.parseString(msgAsString);
  BOOST_CHECK(stringMsg.msg() == answerMsg.msg());
  BOOST_CHECK(stringMsg.format() == answerMsg.format());
}

BOOST_AUTO_TEST_CASE(StringMsgTest)
{
  testStringMsg(msg::FORMAT::JSON);
  testStringMsg(msg::FORMAT::PROTOBUF);
  testStringMsg(msg::FORMAT::XML);
}
