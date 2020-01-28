#include <boost/test/unit_test.hpp>

#include "MsgLib/FinishMsg.hpp"
#include "MsgLib/MsgTypes.hpp"

namespace
{
  void testMsg(const msg::FORMAT& format, const std::string& ans = "")
  {
    msg::FinishMsg msg;

    msg::FinishMsg answerMsg;
    auto msgAsString = msg::toString(msg, format);
    if (ans != "") BOOST_CHECK(ans == msgAsString);
    msg::parseString(answerMsg, msgAsString, format);
    BOOST_CHECK(true);
  }
}

BOOST_AUTO_TEST_CASE(FinishMsgTest)
{
  testMsg(msg::FORMAT::JSON, "{}");
  testMsg(msg::FORMAT::PROTOBUF);
  testMsg(msg::FORMAT::XML, "\n");
}
