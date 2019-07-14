#define BOOST_TEST_MODULE ACQTest
#include <boost/test/unit_test.hpp>

#include "ACQLib/ACQ.hpp"

BOOST_AUTO_TEST_CASE(ACQ)
{
  std::string end;
  std::function<void(std::string const&)> handler([&end](std::string const& toParse) {
    end += toParse;
  });

  AutoConsumedQueue acq(handler);
  std::vector<char> b;
  std::string first("Hello ");
  std::string second("World!");
  std::copy(first.begin(), first.end(), std::back_inserter(b));

  acq.addBytes(b);
  while (acq.isConsuming())
    std::this_thread::yield();
  BOOST_CHECK(end == first);
  std::copy(second.begin(), second.end(), std::back_inserter(b));
  acq.addBytes(b);
  while (acq.isConsuming())
    std::this_thread::yield();
  BOOST_CHECK(end == first + first + second);
}
