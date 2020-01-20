#ifndef POINT_MSG_HPP
#define POINT_MSG_HPP

#include <rapidjson/document.h>
#include <rapidxml/rapidxml.hpp>

namespace proto
{
  class Point;
}

namespace msg
{
  class Point
  {
  public:
    Point();
    Point(const double& x, const double& y);

    double x() const { return m_x; }
    double y() const { return m_y; }

    bool parseFromJson(rapidjson::Value& point);
    bool parseFromProto(const proto::Point& point);
    bool parseFromXml(rapidxml::xml_node<>* point);

    void toJson(rapidjson::Document& doc, rapidjson::Value& point) const;
    void toProto(proto::Point* point) const;
    void toXML(rapidxml::xml_node<>* node) const;

  private:
    double m_x;
    double m_y;
  };

  bool operator==(const Point& lhs, const Point& rhs);
}

#endif
