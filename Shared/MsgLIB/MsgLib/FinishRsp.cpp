#include "FinishRsp.hpp"

#include <string>

#pragma warning(push)
#pragma warning(disable : 4267)
#include "ProtoLib/FinishRsp.pb.h"
#pragma warning(pop)

msg::FinishRsp::FinishRsp()
{
}

bool msg::FinishRsp::parseFromJson(const std::string&)
{
  return true;
}

bool msg::FinishRsp::parseFromProto(const std::string& msg)
{
  return true;
}

bool msg::FinishRsp::parseFromXml(const std::string& msg)
{
  return true;
}

std::string msg::FinishRsp::toJsonString() const
{
  return "{}";
}

std::string msg::FinishRsp::toProtoString() const
{
  proto::FinishRsp msg;
  return msg.SerializeAsString();
}

std::string msg::FinishRsp::toXMLString() const
{
  return "\n";
}
