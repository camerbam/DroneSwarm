#include <boost/test/unit_test.hpp>

#include "MsgLib/MsgTypes.hpp"
#include "MsgLib/TargetMsg.hpp"

namespace
{
  void testMsg(const msg::FORMAT& format)
  {
    msg::TargetMsg msg;
    msg.id(1);
    msg.point({2, 3});

    msg::TargetMsg answerMsg;
    auto msgAsString = msg::toString(msg, format);
    msg::parseString(answerMsg, msgAsString, format);
    BOOST_CHECK(msg.id() == answerMsg.id());
    BOOST_CHECK(msg.point() == answerMsg.point());
  }
}

BOOST_AUTO_TEST_CASE(TargetMsgTest)
{
  testMsg(msg::FORMAT::JSON);
  testMsg(msg::FORMAT::PROTOBUF);
  testMsg(msg::FORMAT::XML);
}
