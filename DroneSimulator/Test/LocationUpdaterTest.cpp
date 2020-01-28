#include <boost/test/unit_test.hpp>

#include <chrono>
#include <iostream>
#include <iomanip>

#include "DroneSimulatorLib/DroneSimulatorConsts.hpp"
#include "DroneSimulatorLib/Updaters/LocationUpdater.hpp"
#include "RegistryLib/Registry.hpp"
#include "UtilsLib/Utils.hpp"

namespace
{
  void updateSecond(drone::LocationUpdater& updater,
                    drone::DroneLocation& location,
                    drone::DroneConfiguration& config)
  {
    for (int i = 0; i < 5; i++)
      updater.updateState(location, config);
  }
}

BOOST_AUTO_TEST_CASE(LocationUpdaterpp)
{
  GlobalRegistry::setRegistry(10, 20);

  drone::DroneLocation target;
  target.setXCoordinate(25);
  target.setYCoordinate(-15);
  target.setZCoordinate(25);
  drone::LocationUpdater updater(target);

  drone::DroneLocation location;
  location.setXCoordinate(-25);
  location.setYCoordinate(15);
  location.setZCoordinate(0);
  drone::DroneConfiguration config;
  updateSecond(updater, location, config);

  BOOST_CHECK(utils::compareTwoDoubles(location.getTimeOfFlight(), 90));
  BOOST_CHECK(utils::compareTwoDoubles(location.getXCoordinate(), -1.19048));
  BOOST_CHECK(utils::compareTwoDoubles(location.getYCoordinate(), 0.714286));
  BOOST_CHECK(utils::compareTwoDoubles(location.getZCoordinate(), 11.904761));

  updateSecond(updater, location, config);

  BOOST_CHECK(utils::compareTwoDoubles(location.getTimeOfFlight(), 140));
  BOOST_CHECK(utils::compareTwoDoubles(location.getXCoordinate(), 22.619047));
  BOOST_CHECK(utils::compareTwoDoubles(location.getYCoordinate(), -13.571428));
  BOOST_CHECK(utils::compareTwoDoubles(location.getZCoordinate(), 23.809523));

  updateSecond(updater, location, config);
  BOOST_CHECK(utils::compareTwoDoubles(location.getTimeOfFlight(), 145));
  BOOST_CHECK(utils::compareTwoDoubles(location.getXCoordinate(), 25));
  BOOST_CHECK(utils::compareTwoDoubles(location.getYCoordinate(), -15));
  BOOST_CHECK(utils::compareTwoDoubles(location.getZCoordinate(), 25));
}

BOOST_AUTO_TEST_CASE(LocationUpdaterpn)
{
  drone::DroneLocation target;
  target.setXCoordinate(55);
  target.setYCoordinate(15);
  target.setZCoordinate(40);
  drone::LocationUpdater updater(target);

  drone::DroneLocation location;
  location.setXCoordinate(5);
  location.setYCoordinate(15);
  location.setZCoordinate(5);
  drone::DroneConfiguration config;
  updateSecond(updater, location, config);

  BOOST_CHECK(utils::compareTwoDoubles(location.getTimeOfFlight(), 75));
  BOOST_CHECK(utils::compareTwoDoubles(location.getXCoordinate(), 34.411764));
  BOOST_CHECK(utils::compareTwoDoubles(location.getYCoordinate(), 15));
  BOOST_CHECK(utils::compareTwoDoubles(location.getZCoordinate(), 25.588235));

  updateSecond(updater, location, config);
  BOOST_CHECK(utils::compareTwoDoubles(location.getTimeOfFlight(), 110));
  BOOST_CHECK(utils::compareTwoDoubles(location.getXCoordinate(), 55));
  BOOST_CHECK(utils::compareTwoDoubles(location.getYCoordinate(), 15));
  BOOST_CHECK(utils::compareTwoDoubles(location.getZCoordinate(), 40));
}

BOOST_AUTO_TEST_CASE(LocationUpdaternp)
{
  drone::DroneLocation target;
  target.setXCoordinate(-25);
  target.setYCoordinate(-15);
  target.setZCoordinate(10);
  drone::DroneConfiguration config;
  drone::LocationUpdater updater(target);

  drone::DroneLocation location;
  location.setXCoordinate(-5);
  location.setYCoordinate(-15);
  location.setZCoordinate(-5);
  updateSecond(updater, location, config);
  BOOST_CHECK(utils::compareTwoDoubles(location.getTimeOfFlight(), 50));
  BOOST_CHECK(utils::compareTwoDoubles(location.getXCoordinate(), -25));
  BOOST_CHECK(utils::compareTwoDoubles(location.getYCoordinate(), -15));
  BOOST_CHECK(utils::compareTwoDoubles(location.getZCoordinate(), 10));
}
