#include "PingMsg.hpp"

#include <rapidjson/document.h>
#include <rapidxml/rapidxml.hpp>

#include "JSONLib/Serialize.hpp"
#include "XMLLib/Serialize.hpp"

#pragma warning(push)
#pragma warning(disable : 4267)
#include "ProtoLib/PingMsg.pb.h"
#pragma warning(pop)

msg::PingMsg::PingMsg() 
{
}

bool msg::PingMsg::parseFromJson(const std::string&)
{
  return true;
}

bool msg::PingMsg::parseFromProto(const std::string&)
{
  return true;
}

bool msg::PingMsg::parseFromXml(const std::string&)
{
  return true;
}

std::string msg::PingMsg::toJsonString() const
{
  return "{}";
}

std::string msg::PingMsg::toProtoString() const
{
  proto::PingMsg msg;
  return msg.SerializeAsString();
}

std::string msg::PingMsg::toXMLString() const
{
  auto pDoc = new rapidxml::xml_document<>;
  auto toReturn = xml::xmlToString(pDoc);
  delete pDoc;
  return toReturn;
}
