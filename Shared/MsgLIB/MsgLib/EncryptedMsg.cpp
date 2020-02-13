#include "EncryptedMsg.hpp"

#include <iostream>
#include <string>

#include <rapidjson/document.h>
#include <rapidxml/rapidxml.hpp>

#include "JSONLib/Deserialize.hpp"
#include "JSONLib/Serialize.hpp"
#include "XMLLib/Deserialize.hpp"
#include "XMLLib/Serialize.hpp"

#pragma warning(push)
#pragma warning(disable : 4267)
#include "ProtoLib/EncryptedMsg.pb.h"
#pragma warning(pop)

namespace
{
  const std::string N_ID("msgId");
  const std::string N_TYPE("type");
  const std::string N_MSG("msg");
} // namespace

msg::EncryptedMsg::EncryptedMsg(const std::string& msgId,
                                const std::string& type,
                                const std::string& msg)
  : m_msgId(msgId), m_type(type), m_msg(msg)
{
}

msg::EncryptedMsg::EncryptedMsg() : m_msgId(), m_type(), m_msg()
{
}

bool msg::EncryptedMsg::parseFromJson(const std::string& msg)
{
  rapidjson::Document json(rapidjson::kObjectType);
  json.Parse(msg.c_str());
  m_msgId = json::getString(json, N_ID);
  m_type = json::getString(json, N_TYPE);
  m_msg = json::getString(json, N_MSG);
  return true;
}

bool msg::EncryptedMsg::parseFromProto(const std::string& msg)
{
  proto::EncryptedMsg m;
  m.ParseFromString(msg);
  m_msgId = m.msgid();
  m_type = m.type();
  m_msg = m.msg();
  return true;
}

bool msg::EncryptedMsg::parseFromXml(const std::string& msg)
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

std::string msg::EncryptedMsg::toJsonString() const
{
  rapidjson::Document doc(rapidjson::kObjectType);
  json::addStringToDoc(doc, N_ID, m_msgId);
  json::addStringToDoc(doc, N_TYPE, m_type);
  json::addStringToDoc(doc, N_MSG, m_msg);
  return json::jsonToString(doc);
}

std::string msg::EncryptedMsg::toProtoString() const
{
  proto::EncryptedMsg msg;
  msg.set_msgid(m_msgId);
  msg.set_type(m_type);
  msg.set_msg(m_msg);
  return msg.SerializeAsString();
}

std::string msg::EncryptedMsg::toXMLString() const
{
  auto pDoc = new rapidxml::xml_document<>;
  xml::addDataToNode(pDoc, N_ID, m_msgId);
  xml::addDataToNode(pDoc, N_TYPE, m_type);
  xml::addDataToNode(pDoc, N_MSG, m_msg);
  auto toReturn = xml::xmlToString(pDoc);
  delete pDoc;
  return toReturn;
}
