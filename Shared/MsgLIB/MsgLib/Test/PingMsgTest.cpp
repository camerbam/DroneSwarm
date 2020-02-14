#include <boost/test/unit_test.hpp>

#include "MsgLib/MsgTypes.hpp"
#include "MsgLib/PingMsg.hpp"

namespace
{
  void testMsg(const msg::FORMAT& format, std::string answer)
  {
    msg::PingMsg msg;

    msg::PingMsg answerMsg;
    auto msgAsString = msg::toString(msg, format);
    if (!answer.empty()) BOOST_CHECK(answer == msgAsString);
    msg::parseString(answerMsg, msgAsString, format);
    BOOST_CHECK(true);
  }
}

BOOST_AUTO_TEST_CASE(PingMsgTest)
{
  testMsg(msg::FORMAT::JSON, "{}");
  testMsg(msg::FORMAT::PROTOBUF, "");
  testMsg(msg::FORMAT::XML, "\n");
}
