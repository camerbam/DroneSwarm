#ifndef FLIGHT_PATH_MSG_HPP
#define FLIGHT_PATH_MSG_HPP

#include "TargetMsg.hpp"
#include <vector>

namespace msg
{
  class FlightPathMsg
  {
  public:
    FlightPathMsg();
    FlightPathMsg(const std::vector<TargetMsg>& m_targets);
    static std::string name() { return "FlightPathMsg"; }

    std::vector<TargetMsg> targets() const { return m_targets; }
    void targets(const std::vector<TargetMsg>& targets) { m_targets = targets; }

    bool parseFromJson(const std::string& msg);
    bool parseFromProto(const std::string& msg);
    bool parseFromXml(const std::string& msg);

    std::string toJsonString() const;
    std::string toProtoString() const;
    std::string toXMLString() const;

  private:
    std::vector<TargetMsg> m_targets;
  };
}

#endif
