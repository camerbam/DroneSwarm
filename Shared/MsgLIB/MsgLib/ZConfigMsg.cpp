#include "ZConfigMsg.hpp"

#include <iostream>
#include <string>

#include <rapidjson/document.h>
#include <rapidxml/rapidxml.hpp>

#include "JSONLib/Deserialize.hpp"
#include "JSONLib/Serialize.hpp"

#pragma warning(push)
#pragma warning(disable : 4267)
#include "ProtoLib/ZConfigMsg.pb.h"
#pragma warning(pop)

#include "XMLLib/Deserialize.hpp"
#include "XMLLib/Serialize.hpp"

namespace
{
  const std::string Z_AXIS("zAxis");
} // namespace

msg::ZConfigMsg::ZConfigMsg(const double& zAxis)
  : m_zAxis(zAxis)
{
}

msg::ZConfigMsg::ZConfigMsg() : m_zAxis() {}

bool msg::ZConfigMsg::parseFromJson(const std::string& msg)
{
  rapidjson::Document json(rapidjson::kObjectType);
  json.Parse(msg.c_str());
  m_zAxis = json::getNumber(json, Z_AXIS);
  return true;
}

bool msg::ZConfigMsg::parseFromProto(const std::string& msg)
{
  proto::ZConfigMsg m;
  m.ParseFromString(msg);
  m_zAxis = m.zaxis();
  return true;
}

bool msg::ZConfigMsg::parseFromXml(const std::string& msg)
{
  auto pDoc = new rapidxml::xml_document<>;
  char* cstr = new char[msg.size() + 1];
  strcpy(cstr, msg.c_str());
  pDoc->parse<0>(cstr);
  m_zAxis = xml::getNumber(pDoc, Z_AXIS);
  delete[] cstr;
  delete pDoc;
  return true;
}

std::string msg::ZConfigMsg::toJsonString() const
{
  rapidjson::Document doc(rapidjson::kObjectType);
  json::addNumberToDoc(doc, Z_AXIS, m_zAxis);
  return json::jsonToString(doc);
}

std::string msg::ZConfigMsg::toProtoString() const
{
  proto::ZConfigMsg msg;
  msg.set_zaxis(m_zAxis);
  return msg.SerializeAsString();
}

std::string msg::ZConfigMsg::toXMLString() const
{
  auto pDoc = new rapidxml::xml_document<>;
  xml::addDataToNode(pDoc, Z_AXIS, m_zAxis);
  auto toReturn = xml::xmlToString(pDoc);
  delete pDoc;
  return toReturn;
}
