#include <boost/test/unit_test.hpp>

#include <vector>

#include "GroundStationLib/GroundStationController.hpp"

BOOST_AUTO_TEST_CASE(GroundStationLibTest)
{
  std::vector<Target> targets;
  targets.emplace_back(0, 20, 1);
  targets.emplace_back(0, 40, 2);
  targets.emplace_back(0, 60, 3);
  targets.emplace_back(0, 80, 4, std::set<int>(), 1);
  targets.emplace_back(0, 100, 5, std::set<int>{3});
  GlobalRegistry::setRegistry(1, 2, targets);

  ground::GroundStationController controller("localhost", "65001");
}
