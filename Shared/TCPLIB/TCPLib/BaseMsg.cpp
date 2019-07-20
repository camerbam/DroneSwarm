#include "BaseMsg.hpp"

#include <string>
#include <iostream>

#include <rapidjson/document.h>

#include "JSONLib/Deserialize.hpp"
#include "JSONLib/Serialize.hpp"

#include "TCPTools.hpp"

#pragma warning(push)
#pragma warning(disable : 4267)
#include "ProtoLib/BaseMsg.pb.h"
#pragma warning(pop)

namespace
{

}

//TODO refactor this function
bool tcp::BaseMsg::parseString(std::string msg)
{
  m_format = getMsgFormat(msg);
  msg::BaseMsg m;
  rapidjson::Document doc;

  switch (m_format)
  {
  case tcp::FORMAT::STRING:
    break;
  case tcp::FORMAT::JSON:
    doc.Parse(msg.c_str());
    m_type = json::getString(doc, "type");
    m_msg = json::getString(doc, "msg");
    break;
  case tcp::FORMAT::PROTOBUF:
    m.ParseFromString(msg);
    m_type = m.type();
    m_msg = m.msg();
    break;
  case tcp::FORMAT::XML:
    break;
  default:
    break;
  }
  return true;
}

std::string tcp::BaseMsg::toString()
{
  switch(m_format)
  {
  case tcp::FORMAT::STRING:
    return std::string();
    break;
  case tcp::FORMAT::JSON:
    return toJsonString();
    break;
  case tcp::FORMAT::PROTOBUF:
    return toProtobufString();
    break;
  case tcp::FORMAT::XML:
    return toXMLString();
    break;
  default:
    return std::string();
    break;
  }
}

std::string tcp::BaseMsg::toJsonString()
{
  rapidjson::Document doc;
  json::addStringToDoc(doc, "type", m_type);
  json::addStringToDoc(doc, "msg", m_msg);
  return m_type + json::jsonToString(doc);
}

std::string tcp::BaseMsg::toProtobufString()
{
  msg::BaseMsg msg;
  msg.set_type(m_type);
  msg.set_msg(m_msg);
  return (char)m_format + msg.SerializeAsString();
}

std::string tcp::BaseMsg::toXMLString()
{
  // TODO need XML Library
  return std::string();
}
