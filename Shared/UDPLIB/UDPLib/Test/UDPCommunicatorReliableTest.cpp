#include <condition_variable>

#include <boost/test/unit_test.hpp>

#include "UDPLib/Response.hpp"
#include "UDPLib/UDPCommunicator.hpp"
#include "UDPLib/UDPCommunicatorReliable.hpp"

namespace
{
  void waitForSignal(
    std::mutex& m, std::condition_variable& cv)
  {
    std::unique_lock<std::mutex> lk(m);
    cv.wait(lk);
  }
}

std::shared_ptr<std::thread> startEcho(const short& port,
  std::condition_variable& cv)
{
  auto validateThread = std::make_shared<std::thread>([port, &cv]() {
    udp::UDPCommunicator receiver(port);

    cv.notify_one();
    bool receiving(true);
    size_t amountReceived(0);
    while (receiving)
    {
      auto response = receiver.receiveMessage();
      amountReceived++;
      receiver.sendMessage(response.getMessage(), response.getEndpoint());
      receiving = response.getMessage().find("quit") == std::string::npos;
    }

    BOOST_CHECK(amountReceived == 10);
  });
  return validateThread;
}

BOOST_AUTO_TEST_CASE(UDPCommunicatorReliableTest)
{
  short port(8888);
  boost::asio::ip::udp::endpoint echoEndpoint(
    boost::asio::ip::address::from_string("127.0.0.1"), port);
  std::condition_variable cv;
  std::mutex m;

  auto validateThread = startEcho(port, cv);
  waitForSignal(m, cv);

  udp::UDPCommunicatorReliable sender;

  sender.startPing(
    [&sender, &echoEndpoint]() { sender.sendMessage("test", echoEndpoint); },
    boost::posix_time::milliseconds(20));

  std::this_thread::sleep_for(std::chrono::milliseconds(200));

  sender.sendMessage("quit", echoEndpoint);

  auto response = sender.sendMessage(
    "No Answer", echoEndpoint, boost::posix_time::milliseconds(10));
  BOOST_CHECK(!response.didSucceed());

  validateThread->join();
}
