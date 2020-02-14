#include <boost/test/unit_test.hpp>

#include "MsgLib/BaseMsg.hpp"

namespace
{
  void testMsg(const msg::FORMAT& format, std::string answer = "")
  {
    msg::BaseMsg baseMsg;
    baseMsg.msgId("referee:3");
    baseMsg.type("base");
    baseMsg.msg("test");

    msg::BaseMsg answerMsg;

    auto msgAsString = msg::toString(baseMsg, format);
    if (!answer.empty()) BOOST_CHECK(msgAsString == answer);
    parseString(answerMsg, msgAsString, format);
    BOOST_CHECK(baseMsg.msgId() == answerMsg.msgId());
    BOOST_CHECK(baseMsg.type() == answerMsg.type());
    BOOST_CHECK(baseMsg.msg() == answerMsg.msg());
  }
}

BOOST_AUTO_TEST_CASE(BaseMsgTest)
{
  testMsg(
    msg::FORMAT::JSON, R"({"msgId":"referee:3","type":"base","msg":"test"})");
  testMsg(msg::FORMAT::PROTOBUF);
  testMsg(msg::FORMAT::XML,
          "<msgId>referee:3</msgId>\n<type>base</type>\n<msg>test</msg>\n\n");
}
