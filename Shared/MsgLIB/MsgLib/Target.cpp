#include "Target.hpp"

#include "JSONLib/Deserialize.hpp"
#include "JSONLib/Serialize.hpp"
#include "UtilsLib/Utils.hpp"
#include "XMLLib/Deserialize.hpp"
#include "XMLLib/Serialize.hpp"

#pragma warning(push)
#pragma warning(disable : 4267)
#include "ProtoLib/Target.pb.h"
#pragma warning(pop)

namespace
{
  const std::string N_X("x");
  const std::string N_Y("y");
} // namespace

msg::Target::Target() : m_x(0), m_y(0)
{
}

msg::Target::Target(const int& x, const int& y) : m_x(x), m_y(y)
{
}

bool msg::Target::parseFromJson(rapidjson::Value& obj)
{
  m_x = json::getInt(obj, N_X);
  m_y = json::getInt(obj, N_Y);
  return true;
}

bool msg::Target::parseFromProto(const proto::Target& Target)
{
  m_x = Target.x();
  m_y = Target.y();
  return true;
}

bool msg::Target::parseFromXml(rapidxml::xml_node<>* pTarget)
{
  m_x = static_cast<int>(xml::getNumber(pTarget, N_X));
  m_y = static_cast<int>(xml::getNumber(pTarget, N_Y));
  return true;
}

void msg::Target::toJson(rapidjson::Document& doc,
                         rapidjson::Value& Target) const
{
  json::addIntToObject(doc, Target, N_X, m_x);
  json::addIntToObject(doc, Target, N_Y, m_y);
}

void msg::Target::toProto(proto::Target* pTarget) const
{
  pTarget->set_x(m_x);
  pTarget->set_y(m_y);
}

void msg::Target::toXML(rapidxml::xml_node<>* node) const
{
  xml::addDataToNode(node, N_X, m_x);
  xml::addDataToNode(node, N_Y, m_y);
}

bool msg::operator==(const msg::Target& lhs, const msg::Target& rhs)
{
  return utils::compareTwoDoubles(lhs.x(), rhs.x()) &&
         utils::compareTwoDoubles(lhs.y(), rhs.y());
}
