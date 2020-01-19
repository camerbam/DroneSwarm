#include <boost/test/unit_test.hpp>

#include <chrono>
#include <iostream>

#include "DroneSimulatorLib/DroneSimulatorConsts.hpp"
#include "DroneSimulatorLib/Updaters/LocationUpdater.hpp"
#include "RegistryLib/Registry.hpp"

namespace
{
  void updateSecond(drone::LocationUpdater& updater,
                    drone::DroneLocation& location,
                    drone::DroneConfiguration& config)
  {
    updater.updateState(location, config);
  }

  bool compareDoubles(const double& a, const double& b)
  {
    return std::abs(a - b) < .0001;
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

  BOOST_CHECK(compareDoubles(location.getTimeOfFlight(), 90));
  BOOST_CHECK(compareDoubles(location.getXCoordinate(), -1.19048));
  BOOST_CHECK(compareDoubles(location.getYCoordinate(), 0.714286));
  BOOST_CHECK(compareDoubles(location.getZCoordinate(), 11.9048));

  updateSecond(updater, location, config);

  BOOST_CHECK(compareDoubles(location.getTimeOfFlight(), 140));
  BOOST_CHECK(compareDoubles(location.getXCoordinate(), 22.619));
  BOOST_CHECK(compareDoubles(location.getYCoordinate(), -13.5714));
  BOOST_CHECK(compareDoubles(location.getZCoordinate(), 23.8095));

  updateSecond(updater, location, config);
  BOOST_CHECK(compareDoubles(location.getTimeOfFlight(), 145));
  BOOST_CHECK(compareDoubles(location.getXCoordinate(), 25));
  BOOST_CHECK(compareDoubles(location.getYCoordinate(), -15));
  BOOST_CHECK(compareDoubles(location.getZCoordinate(), 25));
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

  BOOST_CHECK(compareDoubles(location.getTimeOfFlight(), 75));
  BOOST_CHECK(compareDoubles(location.getXCoordinate(), 34.4118));
  BOOST_CHECK(compareDoubles(location.getYCoordinate(), 15));
  BOOST_CHECK(compareDoubles(location.getZCoordinate(), 25.5882));

  updateSecond(updater, location, config);
  BOOST_CHECK(compareDoubles(location.getTimeOfFlight(), 110));
  BOOST_CHECK(compareDoubles(location.getXCoordinate(), 55));
  BOOST_CHECK(compareDoubles(location.getYCoordinate(), 15));
  BOOST_CHECK(compareDoubles(location.getZCoordinate(), 40));
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
  BOOST_CHECK(compareDoubles(location.getTimeOfFlight(), 50));
  BOOST_CHECK(compareDoubles(location.getXCoordinate(), -25));
  BOOST_CHECK(compareDoubles(location.getYCoordinate(), -15));
  BOOST_CHECK(compareDoubles(location.getZCoordinate(), 10));
}
