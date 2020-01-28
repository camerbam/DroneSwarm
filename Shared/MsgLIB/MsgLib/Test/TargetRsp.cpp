#include <boost/test/unit_test.hpp>

#include "MsgLib/MsgTypes.hpp"
#include "MsgLib/TargetRsp.hpp"

namespace
{
  void testMsg(const msg::FORMAT& format, const std::string& ans = "")
  {
    msg::TargetRsp msg;

    msg::TargetRsp answerMsg;
    auto msgAsString = msg::toString(msg, format);
    if (ans != "") BOOST_CHECK(ans == msgAsString);
    msg::parseString(answerMsg, msgAsString, format);
    BOOST_CHECK(true);
  }
}

BOOST_AUTO_TEST_CASE(TargetRspTest)
{
  testMsg(msg::FORMAT::JSON, R"({"error":""})");
  testMsg(msg::FORMAT::PROTOBUF);
  testMsg(msg::FORMAT::XML, "<error/>\n\n");
}
