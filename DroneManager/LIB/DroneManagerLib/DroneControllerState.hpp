#ifndef DRONE_STATE_HPP
#define DRONE_STATE_HPP

#include <atomic>
#include <mutex>
#include <thread>

#include <boost/signals2.hpp>

#include "DroneMessagesLib/DetectionDirection.hpp"
#include "RegistryLib/Target.hpp"

namespace drone
{
  class DroneControllerState
  {
  public:
    DroneControllerState(size_t startBattery = 100);
    ~DroneControllerState();

    void takeoff();
    void land();
    int getMid();
    double getX();
    double getY();
    double getZ();
    double getTimeOfFlight();
    double getAngle();
    void changeSpeed(size_t newSpeed);
    void setDetection(const messages::DETECTION_DIRECTION& direction);
    size_t getSpeed();
    size_t getBattery() const;
    size_t getTime();
    bool isFlying();
    messages::DETECTION_DIRECTION getDirection();
    bool updateStatus(const std::string& statusMessage);
    boost::signals2::scoped_connection registerForBattery(
      std::function<void(size_t)> callback);
    boost::signals2::scoped_connection registerForMid(
      std::function<void(int)> callback);

  private:
    std::atomic<bool> m_flying;
    std::mutex m_statusMutex;
    std::atomic<int> m_mid;
    messages::DETECTION_DIRECTION m_direction;
    std::atomic<double> m_xCoordinate;
    std::atomic<double> m_yCoordinate;
    std::atomic<double> m_zCoordinate;
    std::atomic<double> m_timeOfFlight;
    std::atomic<double> m_angle;
    std::atomic<size_t> m_speed;
    std::atomic<size_t> m_battery;
    std::atomic<size_t> m_time;
    boost::signals2::signal<void(size_t)> m_batterySignal;
    boost::signals2::signal<void(int)> m_midSignal;
    std::vector<Target> m_knownTargets;
  };
} // namespace drone

#endif
