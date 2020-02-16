#include <boost/test/unit_test.hpp>

#include <vector>
#include <condition_variable>

#include "MsgLib/FinishMsg.hpp"
#include "MsgLib/FinishRsp.hpp"
#include "MsgLib/HitTargetMsg.hpp"
#include "MsgLib/HitTargetRsp.hpp"
#include "MsgLib/ReadyMsg.hpp"
#include "MsgLib/ReadyRsp.hpp"
#include "MsgLib/ReadyRspRsp.hpp"
#include "RefereeLib/RefereeController.hpp"
#include "TCPLib/TCPClient.hpp"

BOOST_AUTO_TEST_CASE(RefereeControllerTest)
{
  std::vector<boost::signals2::connection> connections;
  std::vector<Target> targets;
  targets.emplace_back(0, 20, 1);
  targets.emplace_back(0, 40, 2);
  targets.emplace_back(0, 60, 3);
  targets.emplace_back(0, 80, 4, std::set<int>{3});
  GlobalRegistry::setRegistry(1, 2, targets);

  std::condition_variable cv;
  std::mutex m;
  std::atomic<bool> readyRsp(false);
  std::atomic<bool> hitTargetRsp(false);
  std::atomic<bool> finishMsg(false);

  referee::RefereeController controller(65001, msg::FORMAT::PROTOBUF);

  tcp::TcpClient client("localhost", "65001");

  connections.push_back(client.registerHandler<msg::ReadyRsp>(
    [&client, &cv, &readyRsp](const msg::ReadyRsp& msg, const std::string& msgId) {
      client.respond(msg::ReadyRspRsp(msg.gameId()), msgId);
      cv.notify_one();
      readyRsp = true;
    }));

  connections.push_back(client.registerHandler<msg::HitTargetRsp>(
    [&hitTargetRsp, &cv](const msg::HitTargetRsp& msg, const std::string&) {
      BOOST_CHECK(msg.badTargets().empty());
      BOOST_CHECK(msg.newTargets().empty());
      BOOST_CHECK(msg.success());
      BOOST_CHECK(!msg.complete());
      hitTargetRsp = true;
      cv.notify_one();
    }));

  connections.push_back(client.registerHandler<msg::FinishRsp>(
    [&finishMsg, &cv](const msg::FinishRsp& msg, const std::string&) {
      BOOST_CHECK(msg.targets().size() == 3);
      finishMsg = true;
      cv.notify_one();
    }));

  client.ready();

  client.send(msg::ReadyMsg());
  {
    std::unique_lock<std::mutex> lock(m);
    cv.wait_for(lock, std::chrono::seconds(10));
  }

  client.send(msg::HitTargetMsg(0, 2, {0, 40}));
  {
    std::unique_lock<std::mutex> lock(m);
    cv.wait_for(lock, std::chrono::seconds(10));
  }

  client.send(msg::FinishMsg(0));
  {
    std::unique_lock<std::mutex> lock(m);
    cv.wait_for(lock, std::chrono::seconds(10));
  }

  client.close();
  BOOST_CHECK(readyRsp);
  BOOST_CHECK(hitTargetRsp);
  BOOST_CHECK(finishMsg);
}
