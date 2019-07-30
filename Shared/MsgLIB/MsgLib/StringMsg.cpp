#include "StringMsg.hpp"

#include <iostream>
#include <string>

#include <rapidjson/document.h>
#include <rapidxml/rapidxml.hpp>

#include "JSONLib/Deserialize.hpp"
#include "JSONLib/Serialize.hpp"

#pragma warning(push)
#pragma warning(disable : 4267)
#include "ProtoLib/StringMsg.pb.h"
#pragma warning(pop)

#include "XMLLib/Deserialize.hpp"
#include "XMLLib/Serialize.hpp"

namespace
{
  const std::string N_MSG("msg");
} // namespace

msg::StringMsg::StringMsg(const FORMAT& format, const std::string& msg)
  : m_format(format), m_msg(msg)
{
}

msg::StringMsg::StringMsg() : m_format(), m_msg() {}

// TODO refactor this function
bool msg::StringMsg::parseString(std::string& msg)
{
  m_format = getMsgFormat(msg);

  switch (m_format)
  {
  case msg::FORMAT::JSON:
    parseFromJson(msg);
    return true;
  case msg::FORMAT::PROTOBUF:
    parseFromProto(msg);
    return true;
  case msg::FORMAT::XML:
    parseFromXml(msg);
    return true;
  default:
    return false;
    break;
  }
}

std::string msg::StringMsg::toString()
{
  switch (m_format)
  {
  case msg::FORMAT::JSON:
    return toJsonString();
    break;
  case msg::FORMAT::PROTOBUF:
    return toProtoString();
    break;
  case msg::FORMAT::XML:
    return toXMLString();
    break;
  default:
    return std::string();
    break;
  }
}

bool msg::StringMsg::parseFromJson(const std::string& msg)
{
  rapidjson::Document json;
  json.Parse(msg.c_str());
  m_msg = json::getString(json, N_MSG);
  return true;
}

bool msg::StringMsg::parseFromProto(const std::string& msg)
{
  proto::StringMsg m;
  m.ParseFromString(msg.c_str());
  m_msg = m.msg();
  return true;
}

bool msg::StringMsg::parseFromXml(const std::string& msg)
{
  auto pDoc = new rapidxml::xml_document<>;
  char* cstr = new char[msg.size() + 1];
  strcpy(cstr, msg.c_str());
  pDoc->parse<0>(cstr);
  m_msg = xml::getString(pDoc, N_MSG);
  return true;
}

std::string msg::StringMsg::toJsonString()
{
  rapidjson::Document doc;
  json::addStringToDoc(doc, N_MSG, m_msg);
  return (char)m_format + json::jsonToString(doc);
}

std::string msg::StringMsg::toProtoString()
{
  proto::StringMsg msg;
  msg.set_msg(m_msg);
  return (char)m_format + msg.SerializeAsString();
}

std::string msg::StringMsg::toXMLString()
{
  auto pDoc = new rapidxml::xml_document<>;
  xml::addDataToNode(pDoc, N_MSG, m_msg);
  return (char)m_format + xml::xmlToString(pDoc);
}
