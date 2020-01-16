#ifndef DRONE_STATUS_MESSAGE_HPP
#define DRONE_STATUS_MESSAGE_HPP

#include <string>

namespace messages
{
  class DroneStatusMessage
  {
  public:
    DroneStatusMessage();

    std::string toString(int mid,
                         double xCoordinate,
                         double yCoordinate,
                         double zCoordinate,
                         size_t m_timeOfFlight,
                         size_t m_battery,
                         size_t m_time) const;

    void fromString(std::string message);

    int getMid();
    double getXCoordinate();
    double getYCoordinate();
    double getZCoordinate();
    size_t getTimeOfFlight();
    size_t getBattery();
    size_t getTime();

  protected:
    int m_mid;
    double m_xCoordinate;
    double m_yCoordinate;
    double m_zCoordinate;
    size_t m_timeOfFlight;
    size_t m_battery;
    size_t m_time;
  };
} // namespace messages

#endif