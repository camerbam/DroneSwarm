#include "BaseMsg.hpp"

#include <iostream>
#include <string>

#include <rapidjson/document.h>
#include <rapidxml/rapidxml.hpp>

#include "JSONLib/Deserialize.hpp"
#include "JSONLib/Serialize.hpp"

#pragma warning(push)
#pragma warning(disable : 4267)
#include "ProtoLib/BaseMsg.pb.h"
#pragma warning(pop)

#include "XMLLib/Deserialize.hpp"
#include "XMLLib/Serialize.hpp"

namespace
{
  const std::string N_TYPE("type");
  const std::string N_MSG("msg");
} // namespace

msg::BaseMsg::BaseMsg(const std::string& type,
                      const std::string& msg)
  : m_type(type), m_msg(msg)
{
}

msg::BaseMsg::BaseMsg() : m_type(), m_msg() {}

bool msg::BaseMsg::parseFromJson(const std::string& msg)
{
  rapidjson::Document json(rapidjson::kObjectType);
  json.Parse(msg.c_str());
  m_type = json::getString(json, N_TYPE);
  m_msg = json::getString(json, N_MSG);
  return true;
}

bool msg::BaseMsg::parseFromProto(const std::string& msg)
{
  proto::BaseMsg m;
  m.ParseFromString(msg.c_str());
  m_type = m.type();
  m_msg = m.msg();
  return true;
}

bool msg::BaseMsg::parseFromXml(const std::string& msg)
{
  auto pDoc = new rapidxml::xml_document<>;
  char* cstr = new char[msg.size() + 1];
  strcpy(cstr, msg.c_str());
  pDoc->parse<0>(cstr);
  m_type = xml::getString(pDoc, N_TYPE);
  m_msg = xml::getString(pDoc, N_MSG);
  return true;
}

std::string msg::BaseMsg::toJsonString() const
{
  rapidjson::Document doc(rapidjson::kObjectType);
  json::addStringToDoc(doc, N_TYPE, m_type);
  json::addStringToDoc(doc, N_MSG, m_msg);
  return msg::formatToChar(msg::FORMAT::JSON) + json::jsonToString(doc);
}

std::string msg::BaseMsg::toProtoString() const
{
  proto::BaseMsg msg;
  msg.set_type(m_type);
  msg.set_msg(m_msg);
  return msg::formatToChar(msg::FORMAT::PROTOBUF) + msg.SerializeAsString();
}

std::string msg::BaseMsg::toXMLString() const
{
  auto pDoc = new rapidxml::xml_document<>;
  xml::addDataToNode(pDoc, N_TYPE, m_type);
  xml::addDataToNode(pDoc, N_MSG, m_msg);
  return msg::formatToChar(msg::FORMAT::XML) + xml::xmlToString(pDoc);
}
