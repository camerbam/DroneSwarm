#include <boost/test/unit_test.hpp>

#include "MsgLib/HitTargetRsp.hpp"
#include "MsgLib/MsgTypes.hpp"

namespace
{
  void testMsg(const msg::FORMAT& format, const std::string& ans = "")
  {
    msg::HitTargetRsp msg;
    msg.success(true);
    msg.complete(false);
    msg.errorMsg("bad id");
    msg.newTargets({{4, 5}});
    msg.badTargets({{2, 3}});

    msg::HitTargetRsp answerMsg;
    auto msgAsString = msg::toString(msg, format);
    if (ans != "") BOOST_CHECK(ans == msgAsString);
    msg::parseString(answerMsg, msgAsString, format);
    BOOST_CHECK(answerMsg.success() == msg.success());
    BOOST_CHECK(answerMsg.complete() == msg.complete());
    BOOST_CHECK(answerMsg.errorMsg() == msg.errorMsg());
    BOOST_CHECK(answerMsg.newTargets() == msg.newTargets());
    BOOST_CHECK(answerMsg.badTargets() == msg.badTargets());
  }
}

BOOST_AUTO_TEST_CASE(HitTargetRspTest)
{
  testMsg(
    msg::FORMAT::JSON,
    R"({"success":true,"complete":false,"error":"bad id","newTargets":[{"x":4,"y":5}],"badTargets":[{"x":2,"y":3}]})");
  testMsg(msg::FORMAT::PROTOBUF);
  testMsg(
    msg::FORMAT::XML,
    "<success>true</success>\n<complete>false</complete>\n<error>bad "
    "id</error>\n<newTargets>\n\t<x>4</x>\n\t<y>5</y>\n</"
    "newTargets>\n<badTargets>\n\t<x>2</x>\n\t<y>3</y>\n</badTargets>\n\n");
}
