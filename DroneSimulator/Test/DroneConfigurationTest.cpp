#include <boost/test/unit_test.hpp>

#include "DroneSimulatorLib/State/DroneConfiguration.hpp"
#include "RegistryLib/Registry.hpp"

#include <thread>

BOOST_AUTO_TEST_CASE(DRONE_CONFIGURATION_TEST)
{
  auto registry = GlobalRegistry::getRegistry();
  registry.setSpeedRatio(100);
  registry.setBatteryDecaySpeed(1);
  drone::DroneConfiguration config;
  auto now = std::chrono::steady_clock::now();
  BOOST_CHECK_LT(std::chrono::duration_cast<std::chrono::seconds>(
                   now - config.getStartPoint())
                   .count(),
                 1);

  config.setSpeed(100);

  BOOST_CHECK_EQUAL(config.getSpeed(), 100);
  BOOST_CHECK_EQUAL(config.getBattery(), 100);
  BOOST_CHECK_EQUAL(config.getTime(), 0);

  auto fakeNow = now + std::chrono::milliseconds(12);
  config.update(fakeNow);

  BOOST_CHECK_EQUAL(config.getSpeed(), 100);
  BOOST_CHECK_EQUAL(config.getBattery(), 99);
  BOOST_CHECK_EQUAL(config.getTime(), 1);

  drone::DroneConfiguration secondConfig(config);
  BOOST_CHECK_EQUAL(secondConfig.getSpeed(), 100);
  BOOST_CHECK_EQUAL(secondConfig.getBattery(), 99);
  BOOST_CHECK_EQUAL(secondConfig.getTime(), 1);
}

BOOST_AUTO_TEST_CASE(DRONE_CONFIGURATION_DETECTION_TEST)
{
  drone::DroneConfiguration config;
  BOOST_CHECK(config.enableDetection(messages::DETECTION_DIRECTION::DOWN));
  BOOST_CHECK(!config.enableDetection(messages::DETECTION_DIRECTION::BOTH));
  config.disableDetection();
  BOOST_CHECK(config.enableDetection(messages::DETECTION_DIRECTION::DOWN));
}
