#include "FinishRsp.hpp"

#include "JSONLib/Deserialize.hpp"
#include "JSONLib/Serialize.hpp"
#include "XMLLib/Deserialize.hpp"
#include "XMLLib/Serialize.hpp"

#pragma warning(push)
#pragma warning(disable : 4267)
#include "ProtoLib/FinishRsp.pb.h"
#pragma warning(pop)

namespace
{
  const std::string N_TARGETS("targets");
} // namespace

msg::FinishRsp::FinishRsp(const std::vector<Target>& targets)
  : m_targets(targets)
{
}

msg::FinishRsp::FinishRsp() : m_targets()
{
}

bool msg::FinishRsp::parseFromJson(const std::string& msg)
{
  rapidjson::Document json(rapidjson::kObjectType);
  json.Parse(msg.c_str());
  auto points = json::getObjectOrArray(json, N_TARGETS);
  if (!points) return false;
  auto& pointsArray = points.get();
  if (!pointsArray.IsArray()) return false;
  msg::Target tTarget;
  for (auto& point : pointsArray.GetArray())
  {
    if (!tTarget.parseFromJson(point)) return false;
    m_targets.emplace_back(tTarget);
  }
  return true;
}

bool msg::FinishRsp::parseFromProto(const std::string& msg)
{
  proto::FinishRsp m;
  m.ParseFromString(msg);
  msg::Target tTarget;
  for (auto&& p : m.targets())
  {
    if (!tTarget.parseFromProto(p)) return false;
    m_targets.emplace_back(tTarget);
  }
  return true;
}

bool msg::FinishRsp::parseFromXml(const std::string& msg)
{
  auto pDoc = new rapidxml::xml_document<>;
  char* cstr = new char[msg.size() + 1];
  strcpy(cstr, msg.c_str());
  pDoc->parse<0>(cstr);
  auto points = xml::getObject(pDoc, N_TARGETS);
  msg::Target tTarget;

  for (; points != nullptr; points = points->next_sibling(N_TARGETS.c_str()))
  {
    if (!tTarget.parseFromXml(points)) return false;
    m_targets.emplace_back(tTarget);
  }

  delete[] cstr;
  delete pDoc;
  return true;
}

std::string msg::FinishRsp::toJsonString() const
{
  rapidjson::Document doc(rapidjson::kObjectType);
  rapidjson::Value arr(rapidjson::kArrayType);

  msg::Target tTarget;
  for (auto&& p : m_targets)
  {
    rapidjson::Value obj(rapidjson::kObjectType);
    p.toJson(doc, obj);
    json::addObjectToArray(doc, arr, obj);
  }

  json::addArrayToDoc(doc, N_TARGETS, arr);
  return json::jsonToString(doc);
}

std::string msg::FinishRsp::toProtoString() const
{
  proto::FinishRsp msg;
  for (auto&& p : m_targets)
  {
    auto point = msg.mutable_targets()->Add();
    p.toProto(point);
  }
  return msg.SerializeAsString();
}

std::string msg::FinishRsp::toXMLString() const
{
  auto pDoc = new rapidxml::xml_document<>;

  for (auto&& p : m_targets)
  {
    auto pointsNode = xml::addDataToNode(pDoc, N_TARGETS);
    p.toXML(pointsNode);
  }
  auto toReturn = xml::xmlToString(pDoc);
  delete pDoc;
  return toReturn;
}
