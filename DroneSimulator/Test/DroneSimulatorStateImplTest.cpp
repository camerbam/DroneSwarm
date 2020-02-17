#include <boost/test/unit_test.hpp>

#include <condition_variable>
#include <iostream>

#include "DroneSimulatorLib/State/DroneSimulatorStateImpl.hpp"
#include "RegistryLib/Registry.hpp"
#include "UtilsLib/Utils.hpp"

BOOST_AUTO_TEST_CASE(DroneSimulatorStateImplTest)
{
  GlobalRegistry::setRegistry(100, 20, {{0, 0, 0}, {100, 0, 2}});

  drone::DroneSimulatorStateImpl droneState;
  std::condition_variable cv;
  std::mutex mutex;

  auto connection = droneState.registerForCompletedUpdate(
    [&cv](std::string) { cv.notify_one(); });

  BOOST_CHECK(!droneState.land().empty());
  {
    BOOST_CHECK(droneState.takeoff().empty());
    std::unique_lock<std::mutex> lk(mutex);
    cv.wait_for(lk, std::chrono::seconds(5));
  }
  {
    BOOST_CHECK(droneState.land().empty());
    std::unique_lock<std::mutex> lk(mutex);
    cv.wait_for(lk, std::chrono::seconds(5));
  }
  {
    BOOST_CHECK(droneState.takeoff().empty());
    std::unique_lock<std::mutex> lk(mutex);
    cv.wait_for(lk, std::chrono::seconds(5));
  }
  {
    BOOST_CHECK(droneState.changeTargetX(100).empty());
    std::unique_lock<std::mutex> lk(mutex);
    cv.wait_for(lk, std::chrono::seconds(60));
    BOOST_CHECK(utils::compareTwoDoubles(droneState.getX(), 100));
    BOOST_CHECK(droneState.getMid() == 2);
  }
  {
    BOOST_CHECK(droneState.changeTargetY(120).empty());
    std::unique_lock<std::mutex> lk(mutex);
    cv.wait_for(lk, std::chrono::seconds(5));
    BOOST_CHECK(utils::compareTwoDoubles(droneState.getY(), 120));
  }
  {
    BOOST_CHECK(droneState.changeTargetZ(140).empty());
    std::unique_lock<std::mutex> lk(mutex);
    cv.wait_for(lk, std::chrono::seconds(5));
    BOOST_CHECK(utils::compareTwoDoubles(droneState.getZ(), 240));
  }
  {
    BOOST_CHECK(droneState.changeTargetXYZ(40, 50, 60, 30).empty());
    std::unique_lock<std::mutex> lk(mutex);
    cv.wait_for(lk, std::chrono::seconds(5));
    BOOST_CHECK(utils::compareTwoDoubles(droneState.getX(), 140));
    BOOST_CHECK(utils::compareTwoDoubles(droneState.getY(), 170));
    BOOST_CHECK(utils::compareTwoDoubles(droneState.getZ(), 300));
  }
  BOOST_CHECK(utils::compareTwoDoubles(droneState.getTimeOfFlight(), 810));
  {
    BOOST_CHECK(!droneState.changeTargetAngle(.1).empty());
    BOOST_CHECK(!droneState.changeTargetAngle(361).empty());
    BOOST_CHECK(droneState.changeTargetAngle(60).empty());
    std::unique_lock<std::mutex> lk(mutex);
    cv.wait_for(lk, std::chrono::seconds(5));
    BOOST_CHECK(utils::compareTwoDoubles(droneState.getAngle(), 60));
  }
  BOOST_CHECK(droneState.changeSpeed(64).empty());
  BOOST_CHECK_EQUAL(droneState.getSpeed(), 64);

  BOOST_CHECK(
    utils::checkWithin(static_cast<int>(droneState.getBattery()), 92, 2));
  BOOST_CHECK(
    utils::checkWithin(static_cast<int>(droneState.getTime()), 160, 10));

  BOOST_CHECK(!droneState.getStatusMessage().empty());
}
