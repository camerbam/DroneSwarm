#include "Point.hpp"

#include "JSONLib/Deserialize.hpp"
#include "JSONLib/Serialize.hpp"

#pragma warning(push)
#pragma warning(disable : 4267)
#include "ProtoLib/Point.pb.h"
#pragma warning(pop)

#include "XMLLib/Deserialize.hpp"
#include "XMLLib/Serialize.hpp"

namespace
{
  const std::string N_X("x");
  const std::string N_Y("y");
} // namespace

msg::Point::Point() : m_x(0), m_y(0)
{
}

msg::Point::Point(const double& x, const double& y) : m_x(x), m_y(y)
{
}

bool msg::Point::parseFromJson(rapidjson::Value& obj)
{
  m_x = json::getNumber(obj, N_X);
  m_y = json::getNumber(obj, N_Y);
  return true;
}

bool msg::Point::parseFromProto(const proto::Point& point)
{
  m_x = point.x();
  m_y = point.y();
  return true;
}

bool msg::Point::parseFromXml(rapidxml::xml_node<>* pPoint)
{
  m_x = xml::getNumber(pPoint, N_X);
  m_y = xml::getNumber(pPoint, N_Y);
  return true;
}

void msg::Point::toJson(rapidjson::Document& doc, rapidjson::Value& point) const
{
  json::addNumberToObject(doc, point, N_X, m_x);
  json::addNumberToObject(doc, point, N_Y, m_y);
}

void msg::Point::toProto(proto::Point* pPoint) const
{
  pPoint->set_x(m_x);
  pPoint->set_y(m_y);
}

void msg::Point::toXML(rapidxml::xml_node<>* node) const
{
  xml::addDataToNode(node, N_X, m_x);
  xml::addDataToNode(node, N_Y, m_y);
}

bool msg::operator==(const msg::Point& lhs, const msg::Point& rhs)
{
  return lhs.x() == rhs.x() && lhs.y() == rhs.y();
}