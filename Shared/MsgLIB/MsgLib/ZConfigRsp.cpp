#include "ZConfigRsp.hpp"

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
#include "ProtoLib/ZConfigRsp.pb.h"
#pragma warning(pop)

msg::ZConfigRsp::ZConfigRsp()
{
}

bool msg::ZConfigRsp::parseFromJson(const std::string&)
{
  return true;
}

bool msg::ZConfigRsp::parseFromProto(const std::string&)
{
  return true;
}

bool msg::ZConfigRsp::parseFromXml(const std::string&)
{
  return true;
}

std::string msg::ZConfigRsp::toJsonString() const
{
  return "{}";
}

std::string msg::ZConfigRsp::toProtoString() const
{
  proto::ZConfigRsp msg;
  return msg.SerializeAsString();
}

std::string msg::ZConfigRsp::toXMLString() const
{
  auto pDoc = new rapidxml::xml_document<>;
  auto toReturn = xml::xmlToString(pDoc);
  delete pDoc;
  return toReturn;
}
