#define BOOST_TEST_MODULE UDPTest
#include <atomic>
#include <thread>
#include <iostream>

#include <boost/asio/thread_pool.hpp>
#include <boost/signals2/connection.hpp>
#include <boost/test/unit_test.hpp>

#include "UDPLib/UDPSender.hpp"

void startReceiver()
{

  std::thread t1([]() {
    const unsigned short localPort(8080);
    std::atomic<bool> received(false);
    boost::asio::thread_pool pool(1);

    boost::signals2::scoped_connection con;
    {
      udp::UDPSender receiver(pool, localPort);
      con = receiver.registerReceiver([&receiver, &received](
        boost::asio::ip::udp::endpoint endpoint, std::string msg) {
        BOOST_CHECK(msg == "Test Msg");
        BOOST_CHECK(endpoint.port() == 8081);
        receiver.stop();
        received = true;
      });

      receiver.ready();

      while(!received)
        std::this_thread::yield();
    }
    BOOST_CHECK(received);

  });
  t1.detach();
}

BOOST_AUTO_TEST_CASE(UDPSender)
{
  boost::asio::thread_pool pool(1);

  startReceiver();

  boost::asio::ip::udp::endpoint remoteProcess(
    boost::asio::ip::address::from_string("127.0.0.1"), 8080);
  const unsigned short localPort(8081);

  udp::UDPSender sender(pool, localPort);

  sender.ready();

  sender.send("Test Msg", remoteProcess);

  sender.stop();
}