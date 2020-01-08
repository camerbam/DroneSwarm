#ifndef DRONE_CONFIGURATION_HPP
#define DRONE_CONFIGURATION_HPP

#include <atomic>
#include <string>
#include <chrono>

namespace drone
{
  class DroneConfiguration
  {
  public:
    DroneConfiguration(size_t startingBattery = 100);

    DroneConfiguration(const DroneConfiguration& location);

    size_t getSpeed() const;
    size_t getBattery() const;
    size_t getTime() const;
    std::chrono::steady_clock::time_point getStartPoint() const;

    void setSpeed(size_t speed);
    void decreaseBattery();
    void increaseTime();

    void update(const std::chrono::steady_clock::time_point& now);

  private:
    std::atomic<size_t> m_speed;
    std::atomic<size_t> m_battery;
    size_t m_startBattery;
    std::atomic<size_t> m_time;
    const std::chrono::steady_clock::time_point m_startPoint;
  };
} // namespace drone

#endif
