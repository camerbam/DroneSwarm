#ifndef DRONE_STATE_HPP
#define DRONE_STATE_HPP

#include <atomic>
#include <mutex>
#include <thread>

#include <boost/signals2.hpp>

#include "DroneMessagesLib/Messages/BackMessage.hpp"
#include "DroneMessagesLib/Messages/BatteryMessage.hpp"
#include "DroneMessagesLib/Messages/ClockwiseMessage.hpp"
#include "DroneMessagesLib/Messages/CommandMessage.hpp"
#include "DroneMessagesLib/Messages/CounterClockwiseMessage.hpp"
#include "DroneMessagesLib/Messages/DownMessage.hpp"
#include "DroneMessagesLib/Messages/FlipMessage.hpp"
#include "DroneMessagesLib/Messages/ForwardMessage.hpp"
#include "DroneMessagesLib/Messages/GoMessage.hpp"
#include "DroneMessagesLib/Messages/LandMessage.hpp"
#include "DroneMessagesLib/Messages/LeftMessage.hpp"
#include "DroneMessagesLib/Messages/RightMessage.hpp"
#include "DroneMessagesLib/Messages/SpeedMessage.hpp"
#include "DroneMessagesLib/Messages/TakeoffMessage.hpp"
#include "DroneMessagesLib/Messages/TimeMessage.hpp"
#include "DroneMessagesLib/Messages/UpMessage.hpp"

namespace drone
{
  class DroneControllerState
  {
  public:
    DroneControllerState();
    ~DroneControllerState();

    std::string takeoff();
    std::string land();
    std::string changeX(double deltaX);
    double getX();
    std::string changeY(double deltaY);
    double getY();
    std::string changeZ(double deltaZ);
    double getZ();
    std::string changeXYZ(double deltaX, double deltaY, double deltaZ);
    double getTimeOfFlight();
    std::string changeAngle(double deltaAngle);
    double getAngle();
    std::string changeSpeed(size_t newSpeed);
    size_t getSpeed();
    size_t getBattery() const;
    size_t getTime();
    bool updateStatus(std::string& statusMessage);

    boost::signals2::scoped_connection registerForBattery(
      std::function<void(size_t)> callback);

  private:
    std::atomic<bool> m_flying;
    std::mutex m_statusMutex;
    std::atomic<double> m_xCoordinate;
    std::atomic<double> m_yCoordinate;
    std::atomic<double> m_zCoordinate;
    std::atomic<double> m_timeOfFlight;
    std::atomic<double> m_angle;
    std::atomic<size_t> m_speed;
    std::atomic<size_t> m_battery;
    std::atomic<size_t> m_time;
    boost::signals2::signal<void(size_t)> m_batterySignal;
  };
} // namespace drone

#endif
