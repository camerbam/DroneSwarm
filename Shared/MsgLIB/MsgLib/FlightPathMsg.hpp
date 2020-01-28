#ifndef FLIGHT_PATH_MSG_HPP
#define FLIGHT_PATH_MSG_HPP

#include <vector>
#include "Point.hpp"

namespace msg
{
  class FlightPathMsg
  {
  public:
    FlightPathMsg();
    FlightPathMsg(const std::vector<Point>& points);
    static std::string name() { return "FlightPathMsg"; }

    std::vector<Point> points() const { return m_points; }
    void points(const std::vector<Point>& points) { m_points = points; }

    bool parseFromJson(const std::string& msg);
    bool parseFromProto(const std::string& msg);
    bool parseFromXml(const std::string& msg);

    std::string toJsonString() const;
    std::string toProtoString() const;
    std::string toXMLString() const;
  private:
    std::vector<Point> m_points;
  };
}

#endif
