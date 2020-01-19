#include <boost/test/unit_test.hpp>

#include <iostream>

#include "DroneSimulatorLib/State/DroneLocation.hpp"
#include "UtilsLib/Utils.hpp"

BOOST_AUTO_TEST_CASE(DRONE_LOCATION_TEST)
{
  drone::DroneLocation location;
  
  location.setXCoordinate(50);
  location.setYCoordinate(55);
  location.setZCoordinate(60);
  location.setAngle(100);

  BOOST_CHECK(utils::compareTwoDoubles(location.getTimeOfFlight(), 165));
  BOOST_CHECK(utils::compareTwoDoubles(location.getXCoordinate(), 50));
  BOOST_CHECK(utils::compareTwoDoubles(location.getYCoordinate(), 55));
  BOOST_CHECK(utils::compareTwoDoubles(location.getZCoordinate(), 60));
  BOOST_CHECK(utils::compareTwoDoubles(location.getAngle(), 100));

  location.changeXCoordinate(-20);
  location.changeYCoordinate(-35);
  location.changeZCoordinate(-25);
  location.changeAngle(-40);

  BOOST_CHECK(utils::compareTwoDoubles(location.getTimeOfFlight(), 245));
  BOOST_CHECK(utils::compareTwoDoubles(location.getXCoordinate(), 30));
  BOOST_CHECK(utils::compareTwoDoubles(location.getYCoordinate(), 20));
  BOOST_CHECK(utils::compareTwoDoubles(location.getZCoordinate(), 35));
  BOOST_CHECK(utils::compareTwoDoubles(location.getAngle(), 60));

  location.changeAngle(-80);
  BOOST_CHECK(utils::compareTwoDoubles(location.getAngle(), 340));

  location.changeAngle(30);
  BOOST_CHECK(utils::compareTwoDoubles(location.getAngle(), 10));

  drone::DroneLocation secondLocation = location;
  BOOST_CHECK(utils::compareTwoDoubles(secondLocation.getTimeOfFlight(), 245));
  BOOST_CHECK(utils::compareTwoDoubles(secondLocation.getXCoordinate(), 30));
  BOOST_CHECK(utils::compareTwoDoubles(secondLocation.getYCoordinate(), 20));
  BOOST_CHECK(utils::compareTwoDoubles(secondLocation.getZCoordinate(), 35));
  BOOST_CHECK(utils::compareTwoDoubles(secondLocation.getAngle(), 10));
}

BOOST_AUTO_TEST_CASE(DRONE_LOCATION_COMPLEX_TEST)
{
  drone::DroneLocation location;

  location.setXYZCoordinate(50, 51, 52);

  BOOST_CHECK(location.getTimeOfFlight() > 88 && location.getTimeOfFlight() < 89);
  BOOST_CHECK(utils::compareTwoDoubles(location.getXCoordinate(), 50));
  BOOST_CHECK(utils::compareTwoDoubles(location.getYCoordinate(), 51));
  BOOST_CHECK(utils::compareTwoDoubles(location.getZCoordinate(), 52));

  location.changeXYZCoordinate(50, 51, 52);
  BOOST_CHECK(location.getTimeOfFlight() > 176 && location.getTimeOfFlight() < 177);
  BOOST_CHECK(utils::compareTwoDoubles(location.getXCoordinate(), 100));
  BOOST_CHECK(utils::compareTwoDoubles(location.getYCoordinate(), 102));
  BOOST_CHECK(utils::compareTwoDoubles(location.getZCoordinate(), 104));
}
