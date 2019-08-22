#define BOOST_TEST_MODULE ACQTest
#include <boost/test/unit_test.hpp>

#include "ACQLib/ACQ.hpp"
#include "ACQLib/ACQEndpoint.hpp"

BOOST_AUTO_TEST_CASE(ACQString)
{
  std::string end;
  std::function<void(std::string const&)> handler(
    [&end](std::string const& toParse) { end += toParse; });

  AutoConsumedQueue acq(handler);
  acq.ready();
  std::string first("Hello ");
  std::string second("World!");

  acq.add(first);
  while (acq.isConsuming())
    std::this_thread::yield();
  BOOST_CHECK(end == first);
  acq.add(second);
  while (acq.isConsuming())
    std::this_thread::yield();
  BOOST_CHECK(end == first + second);
}

BOOST_AUTO_TEST_CASE(ACQEndpoint)
{
  std::string end;
  boost::asio::ip::udp::endpoint endpoint;
  std::function<void(
    boost::asio::ip::udp::endpoint endpoint, std::string const&)>
  handler([&end](boost::asio::ip::udp::endpoint const&,
                 std::string const& toParse) { end += toParse; });

  AutoConsumedQueueEndpoint acq(handler);
  acq.ready();
  std::string first("Hello ");
  std::string second("World!");

  acq.add(endpoint, first);
  while (acq.isConsuming())
    std::this_thread::yield();
  BOOST_CHECK(end == first);
  acq.add(endpoint, second);
  while (acq.isConsuming())
    std::this_thread::yield();
  BOOST_CHECK(end == first + second);
}
