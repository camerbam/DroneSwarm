#include <boost/test/unit_test.hpp>

#include "MsgLib/MsgTypes.hpp"
#include "MsgLib/PingRsp.hpp"

namespace
{
  void testMsg(const msg::FORMAT& format, std::string answer)
  {
    msg::PingRsp msg;

    msg::PingRsp answerMsg;
    auto msgAsString = msg::toString(msg, format);
    if (!answer.empty()) BOOST_CHECK(answer == msgAsString);
    msg::parseString(answerMsg, msgAsString, format);
    BOOST_CHECK(true);
  }
}

BOOST_AUTO_TEST_CASE(PingRspTest)
{
  testMsg(msg::FORMAT::JSON, "{}");
  testMsg(msg::FORMAT::PROTOBUF, "");
  testMsg(msg::FORMAT::XML, "\n");
}
