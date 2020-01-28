#include "TargetMsg.hpp"

#include <string>

#include <rapidjson/document.h>
#include <rapidxml/rapidxml.hpp>

#include "JSONLib/Deserialize.hpp"
#include "JSONLib/Serialize.hpp"
#include "XMLLib/Deserialize.hpp"
#include "XMLLib/Serialize.hpp"

#pragma warning(push)
#pragma warning(disable : 4267)
#include "ProtoLib/TargetMsg.pb.h"
#pragma warning(pop)

namespace
{
  const std::string N_POINT("point");
  const std::string N_ID("id");
} // namespace

msg::TargetMsg::TargetMsg(const int& id, const Point& point)
  : m_id(id), m_point(point)
{
}

msg::TargetMsg::TargetMsg() : m_id(0), m_point()
{
}

bool msg::TargetMsg::parseFromJson(const std::string& msg)
{
  rapidjson::Document json(rapidjson::kObjectType);
  json.Parse(msg.c_str());
  auto optPoint = json::getObjectOrArray(json, N_POINT);
  if (!optPoint) return false;
  auto& point = optPoint.get();
  if (!m_point.parseFromJson(point)) return false;
  m_id = json::getInt(json, N_ID);
  return true;
}

bool msg::TargetMsg::parseFromProto(const std::string& msg)
{
  proto::TargetMsg m;
  m.ParseFromString(msg);
  m_id = m.id();
  return m_point.parseFromProto(m.point());
}

bool msg::TargetMsg::parseFromXml(const std::string& msg)
{
  auto pDoc = new rapidxml::xml_document<>;
  char* cstr = new char[msg.size() + 1];
  strcpy(cstr, msg.c_str());
  pDoc->parse<0>(cstr);
  auto point = xml::getObject(pDoc, N_POINT);

  if (!m_point.parseFromXml(point)) return false;
  m_id = static_cast<int>(xml::getNumber(pDoc, N_ID));

  delete[] cstr;
  delete pDoc;
  return true;
}

std::string msg::TargetMsg::toJsonString() const
{
  rapidjson::Document doc(rapidjson::kObjectType);
  rapidjson::Value obj(rapidjson::kObjectType);
  m_point.toJson(doc, obj);
  json::addObjectToDoc(doc, N_POINT, obj);
  json::addIntToDoc(doc, N_ID, m_id);
  return json::jsonToString(doc);
}

std::string msg::TargetMsg::toProtoString() const
{
  proto::TargetMsg msg;
  m_point.toProto(msg.mutable_point());
  msg.set_id(m_id);
  return msg.SerializeAsString();
}

std::string msg::TargetMsg::toXMLString() const
{
  auto pDoc = new rapidxml::xml_document<>;
  auto pointNode = xml::addDataToNode(pDoc, N_POINT);
  m_point.toXML(pointNode);
  xml::addDataToNode(pDoc, N_ID, m_id);
  auto toReturn = xml::xmlToString(pDoc);
  delete pDoc;
  return toReturn;
}
