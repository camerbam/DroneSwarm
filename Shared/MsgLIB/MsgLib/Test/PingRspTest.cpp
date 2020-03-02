#include <boost/test/unit_test.hpp>

#include "MsgLib/MsgTypes.hpp"
#include "MsgLib/PingRsp.hpp"

namespace
{
  void testMsg(const msg::FORMAT& format, std::string answer)
  {
    msg::PingRsp msg(3);

    msg::PingRsp answerMsg;
    auto msgAsString = msg::toString(msg, format);
    if (!answer.empty()) BOOST_CHECK(answer == msgAsString);
    msg::parseString(answerMsg, msgAsString, format);
    BOOST_CHECK(true);
  }
}

BOOST_AUTO_TEST_CASE(PingRspTest)
{
  testMsg(msg::FORMAT::JSON, R"({"gameId":3})");
  testMsg(msg::FORMAT::PROTOBUF, "");
  testMsg(msg::FORMAT::XML, "<gameId>3</gameId>\n\n");
}
