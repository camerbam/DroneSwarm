#include <boost/test/unit_test.hpp>

#include "MsgLib/BaseMsg.hpp"

void testBaseMsg(const msg::FORMAT& format)
{
  msg::BaseMsg baseMsg;
  baseMsg.msg("test");
  baseMsg.type("base");
  baseMsg.format(format);

  msg::BaseMsg answerMsg;
  auto msgAsString = baseMsg.toString();
  answerMsg.parseString(msgAsString);
  BOOST_CHECK(baseMsg.msg() == answerMsg.msg());
  BOOST_CHECK(baseMsg.type() == answerMsg.type());
  BOOST_CHECK(baseMsg.format() == answerMsg.format());
}

BOOST_AUTO_TEST_CASE(BaseMsgTest)
{
  testBaseMsg(msg::FORMAT::JSON);
  testBaseMsg(msg::FORMAT::PROTOBUF);
  testBaseMsg(msg::FORMAT::XML);
}
