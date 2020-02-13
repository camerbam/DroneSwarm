#include "PingRsp.hpp"

#include <rapidjson/document.h>
#include <rapidxml/rapidxml.hpp>

#include "JSONLib/Serialize.hpp"
#include "XMLLib/Serialize.hpp"

#pragma warning(push)
#pragma warning(disable : 4267)
#include "ProtoLib/PingRsp.pb.h"
#pragma warning(pop)

msg::PingRsp::PingRsp() 
{
}

bool msg::PingRsp::parseFromJson(const std::string&)
{
  return true;
}

bool msg::PingRsp::parseFromProto(const std::string&)
{
  return true;
}

bool msg::PingRsp::parseFromXml(const std::string&)
{
  return true;
}

std::string msg::PingRsp::toJsonString() const
{
  return "{}";
}

std::string msg::PingRsp::toProtoString() const
{
  proto::PingRsp msg;
  return msg.SerializeAsString();
}

std::string msg::PingRsp::toXMLString() const
{
  auto pDoc = new rapidxml::xml_document<>;
  auto toReturn = xml::xmlToString(pDoc);
  delete pDoc;
  return toReturn;
}
