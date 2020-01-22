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

msg::TargetRsp::TargetRsp()
{
}

bool msg::TargetRsp::parseFromJson(const std::string&)
{
  return true;
}

bool msg::TargetRsp::parseFromProto(const std::string&)
{
  return true;
}

bool msg::TargetRsp::parseFromXml(const std::string&)
{
  return true;
}

std::string msg::TargetRsp::toJsonString() const
{
  return "{}";
}

std::string msg::TargetRsp::toProtoString() const
{
  proto::TargetRsp msg;
  return msg.SerializeAsString();
}

std::string msg::TargetRsp::toXMLString() const
{
  auto pDoc = new rapidxml::xml_document<>;
  auto toReturn = xml::xmlToString(pDoc);
  delete pDoc;
  return toReturn;
}
