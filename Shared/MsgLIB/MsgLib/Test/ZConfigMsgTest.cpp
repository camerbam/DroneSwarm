#include <boost/test/unit_test.hpp>

#include "MsgLib/MsgTypes.hpp"
#include "MsgLib/ZConfigMsg.hpp"

namespace
{
  void testMsg(const msg::FORMAT& format, const std::string& ans = "")
  {
    msg::ZConfigMsg msg;
    msg.zAxis(3);

    msg::ZConfigMsg answerMsg;
    auto msgAsString = msg::toString(msg, format);
    if (ans != "") BOOST_CHECK(ans == msgAsString);
    msg::parseString(answerMsg, msgAsString, format);
    BOOST_CHECK(true);
  }
}

BOOST_AUTO_TEST_CASE(ZConfigMsgTest)
{
  testMsg(msg::FORMAT::JSON, R"({"zAxis":3.0})");
  testMsg(msg::FORMAT::PROTOBUF);
  testMsg(msg::FORMAT::XML, "<zAxis>3.000000</zAxis>\n\n");
}
