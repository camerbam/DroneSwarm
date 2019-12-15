#include <boost/test/unit_test.hpp>

#include "UDPLib/Response.hpp"
#include "UDPLib/UDPCommunicator.hpp"

std::shared_ptr<std::thread> startValidator(const short& port,
                                            std::atomic<bool>& ready)
{
  auto validateThread = std::make_shared<std::thread>([port, &ready]() {
    udp::UDPCommunicator receiver(port);
    {
      ready = true;
      auto response = receiver.receiveMessage(boost::posix_time::seconds(2));
      BOOST_CHECK(response.didSucceed());
      BOOST_CHECK(response.getMessage() == "command");
      BOOST_CHECK(response.getEndpoint().address().to_string() == "127.0.0.1");
    }
    {
      ready = true;
      auto response = receiver.receiveMessage(boost::posix_time::seconds(2));
      BOOST_CHECK(response.didSucceed());
      BOOST_CHECK(response.getMessage() == "takeoff");
      BOOST_CHECK(response.getEndpoint().address().to_string() == "127.0.0.1");
    }
    {
      ready = true;
      auto response = receiver.receiveMessage(boost::posix_time::seconds(2));
      BOOST_CHECK(response.didSucceed());
      BOOST_CHECK(response.getMessage() == "land");
      BOOST_CHECK(response.getEndpoint().address().to_string() == "127.0.0.1");
    }
  });
  return validateThread;
}

BOOST_AUTO_TEST_CASE(UDPCommunicatorTest)
{
  short port(8890);
  boost::asio::ip::udp::endpoint remoteEndpoint(
    boost::asio::ip::address::from_string("127.0.0.1"), port);
  std::atomic<bool> ready(false);

  auto validateThread = startValidator(port, ready);
  while (!ready)
    ;

  udp::UDPCommunicator sender;

  std::string firstMsg("command");
  sender.sendMessage(firstMsg, remoteEndpoint);
  ready = false;
  while (!ready)
    ;

  std::string secondMsg("takeoff");
  sender.sendMessage(secondMsg, remoteEndpoint);
  ready = false;
  while (!ready)
    ;

  std::string thirdMsg("land");
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

  auto response = sender.receiveMessage(boost::posix_time::seconds(1));
  BOOST_CHECK(!response.didSucceed());
}
