#include <boost/test/unit_test.hpp>

#include "MsgLib/FinishRsp.hpp"
#include "MsgLib/MsgTypes.hpp"

namespace
{
  void testMsg(const msg::FORMAT& format, const std::string& answer)
  {
    msg::FinishRsp msg;
    msg.targets({{4, 5}});

    msg::FinishRsp answerMsg;
    auto msgAsString = msg::toString(msg, format);

    if (answer != "") BOOST_CHECK(answer == msgAsString);
    msg::parseString(answerMsg, msgAsString, format);
    BOOST_CHECK(msg.targets() == answerMsg.targets());
  }
}

BOOST_AUTO_TEST_CASE(FinishRspTest)
{
  testMsg(msg::FORMAT::JSON, R"({"targets":[{"x":4,"y":5}]})");
  testMsg(msg::FORMAT::PROTOBUF, "");
  testMsg(
    msg::FORMAT::XML, "<targets>\n\t<x>4</x>\n\t<y>5</y>\n</targets>\n\n");
}
