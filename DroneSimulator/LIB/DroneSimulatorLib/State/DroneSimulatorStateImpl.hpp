#ifndef DRONE_SIMULATOR_STATE_IMPL_HPP
#define DRONE_SIMULATOR_STATE_IMPL_HPP

#include <atomic>
#include <mutex>
#include <thread>

#include <boost/optional.hpp>
#include <boost/signals2/signal.hpp>

#include "DroneConfiguration.hpp"
#include "DroneLocation.hpp"
#include "DroneMessagesLib/DetectionDirection.hpp"

#include "DroneSimulatorLib/Updaters/DroneUpdater.hpp"

namespace drone
{
  class DroneSimulatorStateImpl
  {
  public:
    DroneSimulatorStateImpl(size_t startingBattery = 100);
    ~DroneSimulatorStateImpl();

    std::string takeoff();
    std::string land();
    std::string changeTargetX(double deltaX);
    double getX();
    std::string changeTargetY(double deltaY);
    double getY();
    std::string changeTargetZ(double deltaZ);
    double getZ();
    std::string changeTargetXYZ(double deltaX,
                                double deltaY,
                                double deltaZ,
                                size_t deltaSpeed);
    double getTimeOfFlight();
    std::string changeTargetAngle(double deltaAngle);
    double getAngle();
    std::string changeSpeed(size_t newSpeed);
    size_t getSpeed();
    size_t getBattery() const;
    size_t getTime();
    std::string getStatusMessage();
    boost::optional<std::string> enableDetection(
      messages::DETECTION_DIRECTION direction =
        messages::DETECTION_DIRECTION::BOTH);
    void disableDetection();

    boost::signals2::scoped_connection registerForCompletedUpdate(
      std::function<void(std::string)> callback);

  private:
    void startUpdate();

    std::atomic<bool> m_flying;
    std::mutex m_statusMutex;
    DroneLocation m_currentLocation;
    DroneConfiguration m_configuration;
    std::shared_ptr<DroneUpdater> m_pUpdater;
    boost::signals2::signal<void(std::string)> m_updateSignal;
    std::atomic<bool> m_isRunning;
    std::thread m_updateThread;
  };
} // namespace drone

#endif
