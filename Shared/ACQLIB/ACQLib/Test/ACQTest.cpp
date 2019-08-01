#define BOOST_TEST_MODULE ACQTest
#include <boost/test/unit_test.hpp>

#include "ACQLib/ACQ.hpp"

BOOST_AUTO_TEST_CASE(ACQString)
{
  std::string end;
  std::function<void(std::string const&)> handler(
    [&end](std::string const& toParse) {
      end += toParse;
    });

  AutoConsumedQueue acq(handler);
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
