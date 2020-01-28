#include "FinishMsg.hpp"

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
#include "ProtoLib/FinishMsg.pb.h"
#pragma warning(pop)


msg::FinishMsg::FinishMsg()
{
}

bool msg::FinishMsg::parseFromJson(const std::string&)
{
  return true;
}

bool msg::FinishMsg::parseFromProto(const std::string&)
{
  return true;
}

bool msg::FinishMsg::parseFromXml(const std::string&)
{
  return true;
}

std::string msg::FinishMsg::toJsonString() const
{
  return "{}";
}

std::string msg::FinishMsg::toProtoString() const
{
  proto::FinishMsg msg;
  return msg.SerializeAsString();
}

std::string msg::FinishMsg::toXMLString() const
{
  return "\n";
}
