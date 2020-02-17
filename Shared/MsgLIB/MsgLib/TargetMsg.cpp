#include "TargetMsg.hpp"

#include "JSONLib/Deserialize.hpp"
#include "JSONLib/Serialize.hpp"
#include "UtilsLib/Utils.hpp"
#include "XMLLib/Deserialize.hpp"
#include "XMLLib/Serialize.hpp"

#pragma warning(push)
#pragma warning(disable : 4267)
#include "ProtoLib/Target.pb.h"
#pragma warning(pop)

#include "RegistryLib/Target.hpp"

namespace
{
  const std::string N_X("x");
  const std::string N_Y("y");
} // namespace

msg::TargetMsg::TargetMsg() : m_x(0), m_y(0)
{
}

msg::TargetMsg::TargetMsg(const Target& target)
  : m_x(target.getX()), m_y(target.getY())
{
}

msg::TargetMsg::TargetMsg(const int& x, const int& y) : m_x(x), m_y(y)
{
}

bool msg::TargetMsg::parseFromJson(rapidjson::Value& obj)
{
  m_x = json::getInt(obj, N_X);
  m_y = json::getInt(obj, N_Y);
  return true;
}

bool msg::TargetMsg::parseFromProto(const proto::Target& TargetMsg)
{
  m_x = TargetMsg.x();
  m_y = TargetMsg.y();
  return true;
}

bool msg::TargetMsg::parseFromXml(rapidxml::xml_node<>* pTargetMsg)
{
  m_x = static_cast<int>(xml::getNumber(pTargetMsg, N_X));
  m_y = static_cast<int>(xml::getNumber(pTargetMsg, N_Y));
  return true;
}

void msg::TargetMsg::toJson(rapidjson::Document& doc,
                            rapidjson::Value& TargetMsg) const
{
  json::addIntToObject(doc, TargetMsg, N_X, m_x);
  json::addIntToObject(doc, TargetMsg, N_Y, m_y);
}

void msg::TargetMsg::toProto(proto::Target* pTargetMsg) const
{
  pTargetMsg->set_x(m_x);
  pTargetMsg->set_y(m_y);
}

void msg::TargetMsg::toXML(rapidxml::xml_node<>* node) const
{
  xml::addDataToNode(node, N_X, m_x);
  xml::addDataToNode(node, N_Y, m_y);
}

bool msg::operator==(const msg::TargetMsg& lhs, const msg::TargetMsg& rhs)
{
  return utils::compareTwoDoubles(lhs.x(), rhs.x()) &&
         utils::compareTwoDoubles(lhs.y(), rhs.y());
}
