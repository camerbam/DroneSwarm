#include <boost/test/unit_test.hpp>

#include "MsgLib/MsgTypes.hpp"
#include "MsgLib/ReadyRsp.hpp"

namespace
{
  void testMsg(const msg::FORMAT& format, const std::string& ans = "")
  {
    msg::ReadyRsp msg;
    msg.gameId(3);
    msg.targets({{5, 6}});

    msg::ReadyRsp answerMsg;
    auto msgAsString = msg::toString(msg, format);
    if (ans != "") BOOST_CHECK(ans == msgAsString);
    msg::parseString(answerMsg, msgAsString, format);
    BOOST_CHECK(msg.gameId() == answerMsg.gameId());
    BOOST_CHECK(msg.targets() == answerMsg.targets());
  }
}

BOOST_AUTO_TEST_CASE(ReadyRspTest)
{
  testMsg(msg::FORMAT::JSON, R"({"gameId":3,"targets":[{"x":5,"y":6}]})");
  testMsg(msg::FORMAT::PROTOBUF);
  testMsg(
    msg::FORMAT::XML,
    "<gameId>3</gameId>\n<targets>\n\t<x>5</x>\n\t<y>6</y>\n</targets>\n\n");
}
