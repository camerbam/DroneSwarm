#ifndef DRONE_LOCATION_HPP
#define DRONE_LOCATION_HPP

#include <atomic>
#include <cmath>

namespace drone
{
  class DroneLocation
  {
  public:
    DroneLocation();

    DroneLocation(const DroneLocation& location);

    double getTimeOfFlight() const;
    double getMid() const;
    double getXCoordinate() const;
    double getYCoordinate() const;
    double getZCoordinate() const;
    double getAngle() const;

    void setMid(double x);
    void setXCoordinate(double x);
    void setYCoordinate(double y);
    void setZCoordinate(double z);
    void setXYZCoordinate(double x, double y, double z);
    void setAngle(double angle);

    void changeXCoordinate(double x);
    void changeYCoordinate(double y);
    void changeZCoordinate(double z);
    void changeXYZCoordinate(double x, double y, double z);
    void changeAngle(double angle);

  private:
    void addTimeOfFlight(double timeToAdd);

    std::atomic<double> m_timeOfFlight;
    std::atomic<double> m_mid;
    std::atomic<double> m_xCoordinate;
    std::atomic<double> m_yCoordinate;
    std::atomic<double> m_zCoordinate;
    std::atomic<double> m_angle;
  };
} // namespace drone

#endif
