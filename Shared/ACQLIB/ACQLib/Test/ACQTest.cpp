#define BOOST_TEST_MODULE ACQTest
#include <boost/test/unit_test.hpp>

#include "ACQLib/ACQ.hpp"

BOOST_AUTO_TEST_CASE(ACQ)
{
  std::string end;
  std::function<void(std::string&)> handler([&end](std::string& toParse) {
    end += toParse;
    toParse = "";
  });

  AutoConsumedQueue acq(handler);
  boost::asio::streambuf buf;
  boost::asio::streambuf b;
  std::ostream os(&b);
  std::string first("Hello ");
  std::string second("World!");
  os << first;

  acq.addBytes(b);
  while (acq.isConsuming())
    std::this_thread::yield();
  BOOST_CHECK(end == first);
  os << second;
  acq.addBytes(b);
  while (acq.isConsuming())
    std::this_thread::yield();
  BOOST_CHECK(end == first + first + second);
}
