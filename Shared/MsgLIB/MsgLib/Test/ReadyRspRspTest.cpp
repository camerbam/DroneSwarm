#include <boost/test/unit_test.hpp>

#include "MsgLib/MsgTypes.hpp"
#include "MsgLib/ReadyRspRsp.hpp"

namespace
{
  void testMsg(const msg::FORMAT& format, const std::string& ans = "")
  {
    msg::ReadyRspRsp msg;
    msg.gameId(4);

    msg::ReadyRspRsp answerMsg;
    auto msgAsString = msg::toString(msg, format);
    if (ans != "") BOOST_CHECK(ans == msgAsString);
    msg::parseString(answerMsg, msgAsString, format);
    BOOST_CHECK(msg.gameId() == answerMsg.gameId());
  }
}

BOOST_AUTO_TEST_CASE(ReadyRspRspTest)
{
  testMsg(msg::FORMAT::JSON, R"({"gameId":4})");
  testMsg(msg::FORMAT::PROTOBUF);
  testMsg(msg::FORMAT::XML, "<gameId>4</gameId>\n\n");
}
