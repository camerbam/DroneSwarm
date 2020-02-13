#include "FlightPathRsp.hpp"

#include <rapidjson/document.h>
#include <rapidxml/rapidxml.hpp>

#include "XMLLib/Serialize.hpp"

#pragma warning(push)
#pragma warning(disable : 4267)
#include "ProtoLib/FlightPathRsp.pb.h"
#pragma warning(pop)

msg::FlightPathRsp::FlightPathRsp() 
{
}

bool msg::FlightPathRsp::parseFromJson(const std::string&)
{
  return true;
}

bool msg::FlightPathRsp::parseFromProto(const std::string&)
{
  return true;
}

bool msg::FlightPathRsp::parseFromXml(const std::string&)
{
  return true;
}

std::string msg::FlightPathRsp::toJsonString() const
{
  return "{}";
}

std::string msg::FlightPathRsp::toProtoString() const
{
  proto::FlightPathRsp msg;
  return msg.SerializeAsString();
}

std::string msg::FlightPathRsp::toXMLString() const
{
  auto pDoc = new rapidxml::xml_document<>;
  auto toReturn = xml::xmlToString(pDoc);
  delete pDoc;
  return toReturn;
}
