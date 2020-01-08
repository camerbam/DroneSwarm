#include <boost/test/unit_test.hpp>

#include "MsgLib/BaseMsg.hpp"

void testBaseMsg(const msg::FORMAT& format)
{
  msg::BaseMsg baseMsg;
  baseMsg.msg("test");
  baseMsg.type("base");

  msg::BaseMsg answerMsg;

  auto msgAsString = msg::toString(baseMsg, format);
  //auto answerFormat = msg::getMsgFormat(msgAsString);
  parseString(answerMsg, msgAsString, format);
  BOOST_CHECK(baseMsg.msg() == answerMsg.msg());
  BOOST_CHECK(baseMsg.type() == answerMsg.type());
}

BOOST_AUTO_TEST_CASE(BaseMsgTest)
{
  testBaseMsg(msg::FORMAT::JSON);
  testBaseMsg(msg::FORMAT::PROTOBUF);
  testBaseMsg(msg::FORMAT::XML);
}
