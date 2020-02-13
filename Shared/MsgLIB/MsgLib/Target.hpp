#ifndef TARGET_MSG_HPP
#define TARGET_MSG_HPP

#include <rapidjson/document.h>
#include <rapidxml/rapidxml.hpp>

namespace proto
{
  class Target;
}

namespace msg
{
  class Target
  {
  public:
    Target();
    Target(const int& x, const int& y);

    int x() const { return m_x; }
    int y() const { return m_y; }

    bool parseFromJson(rapidjson::Value& point);
    bool parseFromProto(const proto::Target& point);
    bool parseFromXml(rapidxml::xml_node<>* point);

    void toJson(rapidjson::Document& doc, rapidjson::Value& point) const;
    void toProto(proto::Target* point) const;
    void toXML(rapidxml::xml_node<>* node) const;

  private:
    int m_x;
    int m_y;
  };

  bool operator==(const Target& lhs, const Target& rhs);
}

#endif
