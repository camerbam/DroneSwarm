#include <boost/test/unit_test.hpp>

#include "DroneSimulatorLib/State/DroneConfiguration.hpp"

#include <thread>

BOOST_AUTO_TEST_CASE(DRONE_CONFIGURATION_TEST)
{
  drone::DroneConfiguration config;
  auto now = std::chrono::steady_clock::now();
  BOOST_CHECK_LT(std::chrono::duration_cast<std::chrono::seconds>(
                   now - config.getStartPoint())
                   .count(),
                 2);

  config.setSpeed(100);
  config.decreaseBattery();
  config.increaseTime();

  BOOST_CHECK_EQUAL(config.getSpeed(), 100);
  BOOST_CHECK_EQUAL(config.getBattery(), 99);
  BOOST_CHECK_EQUAL(config.getTime(), 1);

  std::this_thread::sleep_for(std::chrono::seconds(1));
  config.update(std::chrono::steady_clock::now());

  BOOST_CHECK_EQUAL(config.getSpeed(), 100);
  BOOST_CHECK_EQUAL(config.getBattery(), 100);
  BOOST_CHECK_EQUAL(config.getTime(), 1);

  drone::DroneConfiguration secondConfig(config);
  BOOST_CHECK_EQUAL(secondConfig.getSpeed(), 100);
  BOOST_CHECK_EQUAL(secondConfig.getBattery(), 100);
  BOOST_CHECK_EQUAL(secondConfig.getTime(), 1);
}
