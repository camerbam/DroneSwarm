#include "ReadyMsg.hpp"

#include <iostream>
#include <string>

#include <rapidjson/document.h>
#include <rapidxml/rapidxml.hpp>

#include "JSONLib/Serialize.hpp"
#include "XMLLib/Serialize.hpp"

#pragma warning(push)
#pragma warning(disable : 4267)
#include "ProtoLib/ReadyMsg.pb.h"
#pragma warning(pop)

msg::ReadyMsg::ReadyMsg() 
{
}

bool msg::ReadyMsg::parseFromJson(const std::string& msg)
{
  return true;
}

bool msg::ReadyMsg::parseFromProto(const std::string& msg)
{
  return true;
}

bool msg::ReadyMsg::parseFromXml(const std::string& msg)
{
  return true;
}

std::string msg::ReadyMsg::toJsonString() const
{
  rapidjson::Document doc(rapidjson::kObjectType);
  return json::jsonToString(doc);
}

std::string msg::ReadyMsg::toProtoString() const
{
  proto::ReadyMsg msg;
  return msg.SerializeAsString();
}

std::string msg::ReadyMsg::toXMLString() const
{
  auto pDoc = new rapidxml::xml_document<>;
  auto toReturn = xml::xmlToString(pDoc);
  delete pDoc;
  return toReturn;
}
