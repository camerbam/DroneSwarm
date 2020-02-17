#include <boost/test/unit_test.hpp>

#include "MsgLib/FlightPathMsg.hpp"
#include "MsgLib/MsgTypes.hpp"

namespace
{
  void testMsg(const msg::FORMAT& format, const std::string& ans)
  {
    msg::FlightPathMsg msg;
    msg.targets({{4, 5}});

    msg::FlightPathMsg answerMsg;
    auto msgAsString = msg::toString(msg, format);
    if (!ans.empty()) BOOST_CHECK(ans == msgAsString);
    msg::parseString(answerMsg, msgAsString, format);
    BOOST_CHECK(msg.targets() == answerMsg.targets());
  }
}

BOOST_AUTO_TEST_CASE(FlightPathMsgTest)
{
  testMsg(msg::FORMAT::JSON, R"({"targets":[{"x":4,"y":5}]})");
  testMsg(msg::FORMAT::PROTOBUF, "");
  testMsg(
    msg::FORMAT::XML, "<targets>\n\t<x>4</x>\n\t<y>5</y>\n</targets>\n\n");
}
