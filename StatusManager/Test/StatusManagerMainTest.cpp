#define BOOST_TEST_MODULE StatusManagerTest
#include <boost/test/unit_test.hpp>

#include "StatusManagerLib/StatusManager.hpp"

BOOST_AUTO_TEST_CASE(STATUS_MANAGER_TEST)
{
  std::string ports("101:8881 102:8882");
  drone::StatusManager manager(ports);
  auto parsed = manager.getPorts();
  BOOST_REQUIRE(parsed.size() == 2);
  BOOST_CHECK(parsed["101"] == 8881);
  BOOST_CHECK(parsed["102"] == 8882);
}
