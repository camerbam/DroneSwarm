#include "FlightPathMsg.hpp"

#include <rapidjson/document.h>
#include <rapidxml/rapidxml.hpp>

#include "JSONLib/Deserialize.hpp"
#include "JSONLib/Serialize.hpp"
#include "XMLLib/Deserialize.hpp"
#include "XMLLib/Serialize.hpp"

#pragma warning(push)
#pragma warning(disable : 4267)
#include "ProtoLib/FlightPathMsg.pb.h"
#pragma warning(pop)

#include "TargetMsg.hpp"

namespace
{
  const std::string N_POINTS("targets");
} // namespace

msg::FlightPathMsg::FlightPathMsg(const std::vector<TargetMsg>& targets)
  : m_targets(targets)
{
}

msg::FlightPathMsg::FlightPathMsg() : m_targets()
{
}

bool msg::FlightPathMsg::parseFromJson(const std::string& point)
{
  rapidjson::Document json(rapidjson::kObjectType);
  json.Parse(point.c_str());
  auto targets = json::getObjectOrArray(json, N_POINTS);
  if (!targets) return false;
  auto& targetsArray = targets.get();
  if (!targetsArray.IsArray()) return false;
  msg::TargetMsg tTarget;
  for (auto& point : targetsArray.GetArray())
  {
    if (!tTarget.parseFromJson(point)) return false;
    m_targets.emplace_back(tTarget);
  }
  return true;
}

bool msg::FlightPathMsg::parseFromProto(const std::string& msg)
{
  proto::FlightPathMsg m;
  m.ParseFromString(msg);
  msg::TargetMsg tTarget;
  for (auto&& p : m.targets())
  {
    if (!tTarget.parseFromProto(p)) return false;
    m_targets.emplace_back(tTarget);
  }
  return true;
}

bool msg::FlightPathMsg::parseFromXml(const std::string& msg)
{
  auto pDoc = new rapidxml::xml_document<>;
  char* cstr = new char[msg.size() + 1];
  strcpy(cstr, msg.c_str());
  pDoc->parse<0>(cstr);
  auto targets = xml::getObject(pDoc, N_POINTS);
  msg::TargetMsg tTarget;

  for (; targets != nullptr; targets = targets->next_sibling(N_POINTS.c_str()))
  {
    if (!tTarget.parseFromXml(targets)) return false;
    m_targets.emplace_back(tTarget);
  }

  delete[] cstr;
  delete pDoc;
  return true;
}

std::string msg::FlightPathMsg::toJsonString() const
{
  rapidjson::Document doc(rapidjson::kObjectType);
  rapidjson::Value arr(rapidjson::kArrayType);

  msg::TargetMsg tTarget;
  for (auto&& p : m_targets)
  {
    rapidjson::Value obj(rapidjson::kObjectType);
    p.toJson(doc, obj);
    json::addObjectToArray(doc, arr, obj);
  }

  json::addArrayToDoc(doc, N_POINTS, arr);
  return json::jsonToString(doc);
}

std::string msg::FlightPathMsg::toProtoString() const
{
  proto::FlightPathMsg msg;
  for (auto&& p : m_targets)
  {
    auto point = msg.mutable_targets()->Add();
    p.toProto(point);
  }
  return msg.SerializeAsString();
}

std::string msg::FlightPathMsg::toXMLString() const
{
  auto pDoc = new rapidxml::xml_document<>;

  for (auto&& p : m_targets)
  {
    auto targetsNode = xml::addDataToNode(pDoc, N_POINTS);
    p.toXML(targetsNode);
  }
  auto toReturn = xml::xmlToString(pDoc);
  delete pDoc;
  return toReturn;
}
