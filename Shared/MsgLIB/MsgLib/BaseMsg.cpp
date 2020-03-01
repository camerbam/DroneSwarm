#include "BaseMsg.hpp"

#include <iostream>
#include <fstream>
#include <string>

#include <rapidjson/document.h>
#include <rapidxml/rapidxml.hpp>

#include "JSONLib/Deserialize.hpp"
#include "JSONLib/Serialize.hpp"
#include "XMLLib/Deserialize.hpp"
#include "XMLLib/Serialize.hpp"

#pragma warning(push)
#pragma warning(disable : 4267)
#include "ProtoLib/BaseMsg.pb.h"
#pragma warning(pop)

namespace
{
  const std::string N_ID("msgId");
  const std::string N_TYPE("type");
  const std::string N_MSG("msg");
} // namespace

msg::BaseMsg::BaseMsg(const std::string& msgId,
                      const std::string& type,
                      const std::string& msg)
  : m_msgId(msgId), m_type(type), m_msg(msg)
{
}

msg::BaseMsg::BaseMsg() : m_msgId(), m_type(), m_msg()
{
}

bool msg::BaseMsg::parseFromJson(const std::string& msg)
{
  rapidjson::Document json(rapidjson::kObjectType);
  std::ofstream fout("test.txt");
  fout << msg << std::endl;
  json.Parse(msg.c_str(), msg.size());
  m_msgId = json::getString(json, N_ID);
  m_type = json::getString(json, N_TYPE);
  m_msg = json::getString(json, N_MSG);
  if (m_msg.empty())
  {
    auto t = json::getObjectOrArray(json, N_MSG);
    if (t)
      m_msg = json::jsonToString(t.get());
    else
      return false;
  }
  return true;
}

bool msg::BaseMsg::parseFromProto(const std::string& msg)
{
  proto::BaseMsg m;
  m.ParseFromString(msg);
  m_msgId = m.msgid();
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
  m_msgId = xml::getString(pDoc, N_ID);
  m_type = xml::getString(pDoc, N_TYPE);
  m_msg = xml::getString(pDoc, N_MSG);
  delete[] cstr;
  delete pDoc;
  return true;
}

std::string msg::BaseMsg::toJsonString() const
{
  rapidjson::Document doc(rapidjson::kObjectType);
  json::addStringToDoc(doc, N_ID, m_msgId);
  json::addStringToDoc(doc, N_TYPE, m_type);
  json::addStringToDoc(doc, N_MSG, m_msg);
  return json::jsonToString(doc);
}

std::string msg::BaseMsg::toProtoString() const
{
  proto::BaseMsg msg;
  msg.set_msgid(m_msgId);
  msg.set_type(m_type);
  msg.set_msg(m_msg);
  return msg.SerializeAsString();
}

std::string msg::BaseMsg::toXMLString() const
{
  auto pDoc = new rapidxml::xml_document<>;
  xml::addDataToNode(pDoc, N_ID, m_msgId);
  xml::addDataToNode(pDoc, N_TYPE, m_type);
  xml::addDataToNode(pDoc, N_MSG, m_msg);
  auto toReturn = xml::xmlToString(pDoc);
  delete pDoc;
  return toReturn;
}
