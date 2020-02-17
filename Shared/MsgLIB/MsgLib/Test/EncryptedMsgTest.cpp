#include <boost/test/unit_test.hpp>

#include "MsgLib/EncryptedMsg.hpp"

namespace
{
  void testMsg(const msg::FORMAT& format, std::string answer = "")
  {
    msg::EncryptedMsg baseMsg;
    baseMsg.msgId("referee:2");
    baseMsg.type("base");
    baseMsg.msg("test");

    msg::EncryptedMsg answerMsg;

    auto msgAsString = msg::toString(baseMsg, format);
    if (!answer.empty())
    {

      BOOST_CHECK(msgAsString == answer);
    }

    parseString(answerMsg, msgAsString, format);
    BOOST_CHECK(baseMsg.msgId() == answerMsg.msgId());
    BOOST_CHECK(baseMsg.type() == answerMsg.type());
    BOOST_CHECK(baseMsg.msg() == answerMsg.msg());
  }
}

BOOST_AUTO_TEST_CASE(EncyrptedMsgTest)
{
  testMsg(
    msg::FORMAT::JSON, R"({"msgId":"referee:2","type":"base","msg":"test"})");
  testMsg(msg::FORMAT::PROTOBUF);
  testMsg(msg::FORMAT::XML,
          "<msgId>referee:2</msgId>\n<type>base</type>\n<msg>test</msg>\n\n");
}
