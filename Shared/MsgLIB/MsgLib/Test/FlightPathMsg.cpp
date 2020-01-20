#include <boost/test/unit_test.hpp>

#include "MsgLib/FlightPathMsg.hpp"
#include "MsgLib/MsgTypes.hpp"

namespace
{
  void testMsg(const msg::FORMAT& format)
  {
    msg::FlightPathMsg msg;
    msg.points({{1, 2}, {3, 4}});

    msg::FlightPathMsg answerMsg;
    auto msgAsString = msg::toString(msg, format);
    msg::parseString(answerMsg, msgAsString, format);
    BOOST_CHECK(msg.points() == answerMsg.points());
  }
}

BOOST_AUTO_TEST_CASE(FlightPathMsgTest)
{
  testMsg(msg::FORMAT::JSON);
  testMsg(msg::FORMAT::PROTOBUF);
  testMsg(msg::FORMAT::XML);
}
