#include <boost/test/unit_test.hpp>

#include "MsgLib/MsgTypes.hpp"
#include "MsgLib/ReadyMsg.hpp"

namespace
{
  void testMsg(const msg::FORMAT& format, const std::string& ans = "")
  {
    msg::ReadyMsg msg;

    msg::ReadyMsg answerMsg;
    auto msgAsString = msg::toString(msg, format);
    if (ans != "") BOOST_CHECK(ans == msgAsString);
    msg::parseString(answerMsg, msgAsString, format);
    BOOST_CHECK(true);
  }
}

BOOST_AUTO_TEST_CASE(ReadyMsgTest)
{
  testMsg(msg::FORMAT::JSON, "{}");
  testMsg(msg::FORMAT::PROTOBUF);
  testMsg(msg::FORMAT::XML, "\n");
}
