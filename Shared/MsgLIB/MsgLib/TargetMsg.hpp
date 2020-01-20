#ifndef TARGET_MSG_HPP
#define TARGET_MSG_HPP

#include <string>
#include "Point.hpp"

namespace msg
{
  class TargetMsg
  {
  public:
    TargetMsg();
    TargetMsg(const int& id, const Point& point);
    static std::string name() { return "TargetMsg"; }

    int id() const { return m_id; }
    void id(const int& id) { m_id = id; }

    Point point() const { return m_point; }
    void point(const Point& point) { m_point = point; }

    bool parseFromJson(const std::string& msg);
    bool parseFromProto(const std::string& msg);
    bool parseFromXml(const std::string& msg);

    std::string toJsonString() const;
    std::string toProtoString() const;
    std::string toXMLString() const;
  private:
    int m_id;
    Point m_point;
  };
}

#endif
