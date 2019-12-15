#include <boost/test/unit_test.hpp>

#include "DroneControllerLib/DroneControllerState.hpp"

namespace
{
  bool compareTwoDoubles(const double& a, const double& b)
  {
    return std::abs(a - b) < .25;
  }
}

BOOST_AUTO_TEST_CASE(DroneControllerStateTest)
{
  drone::DroneControllerState droneState;
  BOOST_CHECK(droneState.takeoff().empty());
  BOOST_CHECK(!droneState.takeoff().empty());
  BOOST_CHECK(droneState.changeX(20).empty());
  BOOST_CHECK(droneState.changeX(25).empty());
  BOOST_CHECK(compareTwoDoubles(droneState.getX(), 45));
  BOOST_CHECK(droneState.changeY(25).empty());
  BOOST_CHECK(droneState.changeY(30).empty());
  BOOST_CHECK(compareTwoDoubles(droneState.getY(), 55));
  BOOST_CHECK(droneState.changeZ(25).empty());
  BOOST_CHECK(droneState.changeZ(-140).empty());
  BOOST_CHECK(compareTwoDoubles(droneState.getZ(), 0));
  BOOST_CHECK(droneState.changeXYZ(50, 50, 50).empty());
  BOOST_CHECK(droneState.changeAngle(30).empty());
  BOOST_CHECK(droneState.changeAngle(200).empty());
  BOOST_CHECK(droneState.changeAngle(200).empty());
  BOOST_CHECK(droneState.changeAngle(-200).empty());
  BOOST_CHECK(droneState.changeAngle(-200).empty());
  BOOST_CHECK(droneState.changeAngle(-40).empty());
  BOOST_CHECK(!droneState.changeAngle(-430).empty());
  BOOST_CHECK(!droneState.changeAngle(.1).empty());
  BOOST_CHECK(compareTwoDoubles(droneState.getAngle(), 350));
  BOOST_CHECK(droneState.changeSpeed(50).empty());
  BOOST_CHECK(!droneState.changeSpeed(500).empty());
  BOOST_CHECK(!droneState.changeSpeed(5).empty());
  BOOST_CHECK(droneState.getSpeed() == 50);
  BOOST_CHECK(droneState.land().empty());
  BOOST_CHECK(!droneState.land().empty());
  BOOST_CHECK(droneState.getTime() == 0);
}
