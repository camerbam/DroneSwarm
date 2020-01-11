#define BOOST_TEST_MODULE UtilsTest

#include <boost/test/unit_test.hpp>

#include "UtilsLib/Utils.hpp"


BOOST_AUTO_TEST_CASE(CompareDoublesTest)
{
  BOOST_CHECK(utils::compareTwoDoubles(0, 0));
  BOOST_CHECK(utils::compareTwoDoubles(0, .000001));
  BOOST_CHECK(!utils::compareTwoDoubles(0, .00001));
}

BOOST_AUTO_TEST_CASE(CheckWithinTest)
{
  BOOST_CHECK(utils::checkWithin(5, 5, 0));
  BOOST_CHECK(utils::checkWithin(5, 6, 1));
  BOOST_CHECK(!utils::checkWithin(5, 7, 1));
}
