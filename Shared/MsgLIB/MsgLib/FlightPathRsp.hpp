#ifndef FLIGHT_PATH_RSP_HPP
#define FLIGHT_PATH_RSP_HPP

#include <string>

namespace msg
{
  class FlightPathRsp
  {
  public:
    FlightPathRsp();
    static std::string name() { return "FlightPathRsp"; }

    bool parseFromJson(const std::string& msg);
    bool parseFromProto(const std::string& msg);
    bool parseFromXml(const std::string& msg);

    std::string toJsonString() const;
    std::string toProtoString() const;
    std::string toXMLString() const;
  private:
  };
}

#endif
