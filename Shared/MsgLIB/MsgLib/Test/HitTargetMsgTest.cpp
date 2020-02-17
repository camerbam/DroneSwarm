#include <boost/test/unit_test.hpp>

#include "MsgLib/HitTargetMsg.hpp"
#include "MsgLib/MsgTypes.hpp"

namespace
{
  void testMsg(const msg::FORMAT& format, const std::string& answer)
  {
    msg::HitTargetMsg msg;
    msg.gameId(2);
    msg.id(1);
    msg.target({2, 3});

    msg::HitTargetMsg answerMsg;
    auto msgAsString = msg::toString(msg, format);
    if (!answer.empty()) BOOST_CHECK(msgAsString == answer);
    msg::parseString(answerMsg, msgAsString, format);
    BOOST_CHECK(msg.gameId() == answerMsg.gameId());
    BOOST_CHECK(msg.id() == answerMsg.id());
    BOOST_CHECK(msg.target() == answerMsg.target());
  }
}

BOOST_AUTO_TEST_CASE(TargetMsgTest)
{
  testMsg(msg::FORMAT::JSON, R"({"gameId":2,"id":1,"target":{"x":2,"y":3}})");
  testMsg(msg::FORMAT::PROTOBUF, "");
  testMsg(msg::FORMAT::XML,
          "<target>\n\t<x>2</x>\n\t<y>3</y>\n</"
          "target>\n<gameId>2</gameId>\n<id>1</id>\n\n");
}
