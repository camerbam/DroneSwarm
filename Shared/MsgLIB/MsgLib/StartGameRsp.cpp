#include "StartGameRsp.hpp"

#include <rapidjson/document.h>
#include <rapidxml/rapidxml.hpp>

#include "JSONLib/Deserialize.hpp"
#include "JSONLib/Serialize.hpp"
#include "XMLLib/Deserialize.hpp"
#include "XMLLib/Serialize.hpp"

#pragma warning(push)
#pragma warning(disable : 4267)
#include "ProtoLib/StartGameRsp.pb.h"
#pragma warning(pop)

msg::StartGameRsp::StartGameRsp()
{
}

bool msg::StartGameRsp::parseFromJson(const std::string&)
{
  return true;
}

bool msg::StartGameRsp::parseFromProto(const std::string&)
{
  return true;
}

bool msg::StartGameRsp::parseFromXml(const std::string&)
{
  return true;
}

std::string msg::StartGameRsp::toJsonString() const
{
  return "{}";
}

std::string msg::StartGameRsp::toProtoString() const
{
  proto::StartGameRsp msg;
  return msg.SerializeAsString();
}

std::string msg::StartGameRsp::toXMLString() const
{
  auto pDoc = new rapidxml::xml_document<>;
  auto toReturn = xml::xmlToString(pDoc);
  delete pDoc;
  return toReturn;
}
