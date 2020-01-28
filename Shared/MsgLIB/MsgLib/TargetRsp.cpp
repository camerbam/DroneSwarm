#include "TargetRsp.hpp"

#include <rapidjson/document.h>
#include <rapidxml/rapidxml.hpp>

#include "JSONLib/Deserialize.hpp"
#include "JSONLib/Serialize.hpp"
#include "XMLLib/Deserialize.hpp"
#include "XMLLib/Serialize.hpp"

#pragma warning(push)
#pragma warning(disable : 4267)
#include "ProtoLib/TargetRsp.pb.h"
#pragma warning(pop)

#include "Point.hpp"

namespace
{
  const std::string N_ERROR("error");
}

msg::TargetRsp::TargetRsp(const std::string& errorMsg)
  :m_errorMsg(errorMsg)
{
}

bool msg::TargetRsp::parseFromJson(const std::string& msg)
{
  rapidjson::Document json(rapidjson::kObjectType);
  json.Parse(msg.c_str());
  m_errorMsg = json::getString(json, N_ERROR);
  return true;
}

bool msg::TargetRsp::parseFromProto(const std::string& msg)
{
  proto::TargetRsp m;
  m.ParseFromString(msg);
  m_errorMsg = m.error();
  return true;
}

bool msg::TargetRsp::parseFromXml(const std::string& msg)
{
  auto pDoc = new rapidxml::xml_document<>;
  char* cstr = new char[msg.size() + 1];
  strcpy(cstr, msg.c_str());
  pDoc->parse<0>(cstr);
  m_errorMsg = xml::getString(pDoc, N_ERROR);
  delete[] cstr;
  delete pDoc;
  return true;
}

std::string msg::TargetRsp::toJsonString() const
{
  rapidjson::Document doc(rapidjson::kObjectType);
  json::addStringToDoc(doc, N_ERROR, m_errorMsg);
  return json::jsonToString(doc);
}

std::string msg::TargetRsp::toProtoString() const
{
  proto::TargetRsp msg;
  msg.set_error(m_errorMsg);
  return msg.SerializeAsString();
}

std::string msg::TargetRsp::toXMLString() const
{
  auto pDoc = new rapidxml::xml_document<>;
  xml::addDataToNode(pDoc, N_ERROR, m_errorMsg);
  auto toReturn = xml::xmlToString(pDoc);
  delete pDoc;
  return toReturn;
}
