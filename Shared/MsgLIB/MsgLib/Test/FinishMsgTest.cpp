#include <boost/test/unit_test.hpp>

#include "MsgLib/FinishMsg.hpp"
#include "MsgLib/MsgTypes.hpp"

namespace
{
  void testMsg(const msg::FORMAT& format, const std::string& answer = "")
  {
    msg::FinishMsg msg;
    msg.gameId(4);

    msg::FinishMsg answerMsg;
    auto msgAsString = msg::toString(msg, format);
    if (answer != "") BOOST_CHECK(answer == msgAsString);
    msg::parseString(answerMsg, msgAsString, format);
    BOOST_CHECK(msg.gameId() == answerMsg.gameId());
  }
}

BOOST_AUTO_TEST_CASE(FinishMsgTest)
{
  testMsg(msg::FORMAT::JSON, R"({"gameId":4})");
  testMsg(msg::FORMAT::PROTOBUF);
  testMsg(msg::FORMAT::XML, "<gameId>4</gameId>\n\n");
}
