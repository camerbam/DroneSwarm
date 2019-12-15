#include <boost/test/unit_test.hpp>

#include <iostream>

#include "UDPLib/Response.hpp"
#include "UDPLib/UDPCommunicator.hpp"
#include "UDPLib/UDPCommunicatorReliable.hpp"

std::shared_ptr<std::thread> startEcho(const short& port,
                                       std::atomic<bool>& ready)
{
  auto validateThread = std::make_shared<std::thread>([port, &ready]() {
    udp::UDPCommunicator receiver(port);

    ready = true;
    bool receiving(true);
    size_t amountReceived(0);
    while (receiving)
    {
      auto response = receiver.receiveMessage();
      amountReceived++;
      receiver.sendMessage(response.getMessage(), response.getEndpoint());
      receiving = response.getMessage().find("quit") == std::string::npos;
    }
    BOOST_CHECK(amountReceived == 4);
  });
  return validateThread;
}

BOOST_AUTO_TEST_CASE(UDPCommunicatorReliableTest)
{
  short port(8888);
  boost::asio::ip::udp::endpoint echoEndpoint(
    boost::asio::ip::address::from_string("127.0.0.1"), 8888);
  std::atomic<bool> ready(false);

  auto validateThread = startEcho(port, ready);
  while (!ready)
    ;

  udp::UDPCommunicatorReliable sender;

  sender.startPing(
    [&sender, &echoEndpoint]() { sender.sendMessage("test", echoEndpoint); },
    boost::posix_time::milliseconds(250));

  std::this_thread::sleep_for(std::chrono::seconds(1));

  sender.sendMessage("quit", echoEndpoint);

  auto response = sender.sendMessage(
    "No Answer", echoEndpoint, boost::posix_time::milliseconds(250));
  BOOST_CHECK(!response.didSucceed());

  validateThread->join();
}
