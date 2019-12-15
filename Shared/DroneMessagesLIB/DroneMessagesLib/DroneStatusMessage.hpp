#ifndef DRONE_STATUS_MESSAGE_HPP
#define DRONE_STATUS_MESSAGE_HPP

#include <string>

namespace messages
{
  class DroneStatusMessage
  {
  public:
    DroneStatusMessage();

    std::string toString(size_t m_timeOfFlight,
                         double m_zCoordinate,
                         size_t m_battery,
                         size_t m_time) const;

    void fromString(std::string message);

    size_t getTimeOfFlight();
    double getZCoordinate();
    size_t getBattery();
    size_t getTime();

  protected:
    size_t m_timeOfFlight;
    double m_zCoordinate;
    size_t m_battery;
    size_t m_time;
  };
} // namespace messages

#endif