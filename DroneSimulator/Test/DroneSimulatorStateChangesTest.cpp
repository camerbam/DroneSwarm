#include <boost/test/unit_test.hpp>

#include "DroneMessagesLib/MessageFactory.hpp"
#include "DroneSimulatorLib/State/DroneSimulatorStateChanges.hpp"
#include "RegistryLib/Registry.hpp"

#include <condition_variable>
#include <iostream>
#include <mutex>

namespace
{
  void checkMsg(std::condition_variable& cv,
                std::mutex& mutex,
                std::string message,
                std::shared_ptr<drone::DroneSimulatorStateImpl>& pState,
                const boost::optional<std::string>& answer)
  {
    auto response = boost::apply_visitor(
      drone::DroneSimulatorStateChanges(pState), messages::getMessage(message));
    BOOST_CHECK(response == answer);
    std::unique_lock<std::mutex> lk(mutex);
    cv.wait_for(lk, std::chrono::seconds(5));
  }

  void checkRsp(std::string message,
                std::shared_ptr<drone::DroneSimulatorStateImpl>& pState,
                const std::string& answer)
  {
    auto response = boost::apply_visitor(
      drone::DroneSimulatorStateChanges(pState), messages::getMessage(message));
    BOOST_CHECK(response == answer);
  }

  void checkMsg(std::string message,
                std::shared_ptr<drone::DroneSimulatorStateImpl>& pState,
                const int& answer,
                const int& diff)
  {
    auto response = boost::apply_visitor(
      drone::DroneSimulatorStateChanges(pState), messages::getMessage(message));
    BOOST_CHECK(response);
    int responseInt = std::stoi(response.get());
    BOOST_CHECK(responseInt > answer - diff && responseInt < answer + diff);
  }
}

BOOST_AUTO_TEST_CASE(DRONE_SIMULATOR_STATE_CHANGES_TEST)
{
  auto registry = GlobalRegistry::getRegistry();
  registry.setSpeedRatio(100);
  registry.setBatteryDecaySpeed(20);

  auto pState = std::make_shared<drone::DroneSimulatorStateImpl>();
  std::mutex mutex;
  std::condition_variable cv;
  auto connection =
    pState->registerForCompletedUpdate([&cv](std::string) { cv.notify_one(); });

  checkMsg(cv, mutex, "takeoff", pState, boost::none);
  checkMsg(cv, mutex, "back 50", pState, boost::none);
  checkMsg(cv, mutex, "cw 50", pState, boost::none);
  checkRsp("command", pState, std::string("ok"));
  checkMsg(cv, mutex, "ccw 50", pState, boost::none);
  checkMsg(cv, mutex, "down 50", pState, boost::none);
  checkMsg(cv, mutex, "flip f", pState, boost::none);
  checkMsg(cv, mutex, "flip r", pState, boost::none);
  checkMsg(cv, mutex, "flip l", pState, boost::none);
  checkMsg(cv, mutex, "flip b", pState, boost::none);
  checkMsg(cv, mutex, "forward 50", pState, boost::none);
  checkMsg(cv, mutex, "left 50", pState, boost::none);
  checkMsg(cv, mutex, "right 50", pState, boost::none);
  checkRsp("speed?", pState, std::string("10"));
  checkMsg(cv, mutex, "go 50 50 50 50", pState, boost::none);
  checkMsg("battery?", pState, 93, 4);
  checkMsg("time?", pState, 130, 15);
  checkRsp("mon", pState, "ok");
  checkRsp("mdirection 0", pState, "ok");
  checkRsp("moff", pState, "ok");
  checkMsg(cv, mutex, "up 50", pState, boost::none);
  checkMsg(cv, mutex, "land", pState, boost::none);
  checkRsp("up 50", pState, std::string("Drone is not flying"));
}
