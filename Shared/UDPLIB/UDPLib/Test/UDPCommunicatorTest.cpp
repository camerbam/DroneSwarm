#include <boost/test/unit_test.hpp>

#include "UDPLib/Response.hpp"
#include "UDPLib/UDPCommunicator.hpp"

namespace
{
  void waitForSignal(
    std::mutex& m, std::condition_variable& cv)
  {
    std::unique_lock<std::mutex> lk(m);
    cv.wait(lk);
  }
}

std::shared_ptr<std::thread> startValidator(const short& port,
  std::condition_variable& cv)
{
  auto validateThread = std::make_shared<std::thread>([port, &cv]() {
    udp::UDPCommunicator receiver(port);
    cv.notify_one();
    auto response = receiver.receiveMessage(boost::posix_time::seconds(2));
    BOOST_CHECK(response.didSucceed());
    BOOST_CHECK(response.getMessage() == "command");
    BOOST_CHECK(response.getEndpoint().address().to_string() == "127.0.0.1");

    cv.notify_one();
    response = receiver.receiveMessage(boost::posix_time::seconds(2));
    BOOST_CHECK(response.didSucceed());
    BOOST_CHECK(response.getMessage() == "takeoff");
    BOOST_CHECK(response.getEndpoint().address().to_string() == "127.0.0.1");
    
    cv.notify_one();
    response = receiver.receiveMessage(boost::posix_time::seconds(2));
    BOOST_CHECK(response.didSucceed());
    BOOST_CHECK(response.getMessage() == "land");
    BOOST_CHECK(response.getEndpoint().address().to_string() == "127.0.0.1");
  });
  return validateThread;
}

BOOST_AUTO_TEST_CASE(UDPCommunicatorTest)
{
  short port(8890);
  boost::asio::ip::udp::endpoint remoteEndpoint(
    boost::asio::ip::address::from_string("127.0.0.1"), port);
  std::mutex m;
  std::condition_variable cv;

  auto validateThread = startValidator(port, cv);
  
  udp::UDPCommunicator sender;

  std::string firstMsg("command");
  waitForSignal(m, cv);
  sender.sendMessage(firstMsg, remoteEndpoint);

  std::string secondMsg("takeoff");
  waitForSignal(m, cv);
  sender.sendMessage(secondMsg, remoteEndpoint);

  std::string thirdMsg("land");
  waitForSignal(m, cv);
  sender.sendMessage(thirdMsg, remoteEndpoint);

  validateThread->join();
}

BOOST_AUTO_TEST_CASE(UDPNoEndpointTest)
{
  short port(8890);
  boost::asio::ip::udp::endpoint remoteEndpoint(
    boost::asio::ip::address::from_string("127.0.0.1"), port);
  std::string msg("battery?");
  udp::UDPCommunicator sender;

  auto response = sender.receiveMessage(boost::posix_time::milliseconds(10));
  BOOST_CHECK(!response.didSucceed());
}
