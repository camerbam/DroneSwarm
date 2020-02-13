#include "ReadyRsp.hpp"

#include <rapidjson/document.h>
#include <rapidxml/rapidxml.hpp>

#include "JSONLib/Deserialize.hpp"
#include "JSONLib/Serialize.hpp"
#include "XMLLib/Deserialize.hpp"
#include "XMLLib/Serialize.hpp"

#pragma warning(push)
#pragma warning(disable : 4267)
#include "ProtoLib/ReadyRsp.pb.h"
#pragma warning(pop)

namespace
{
  const std::string N_GAME_ID("gameId");
  const std::string N_TARGETS("targets");
}

msg::ReadyRsp::ReadyRsp()
{
}

msg::ReadyRsp::ReadyRsp(int gameId, const std::vector<Target>& targets)
  : m_gameId(gameId), m_targets(targets)
{
}

bool msg::ReadyRsp::parseFromJson(const std::string& msg)
{
  rapidjson::Document json(rapidjson::kObjectType);
  json.Parse(msg.c_str());
  m_gameId = json::getInt(json, N_GAME_ID);

  auto optTarget = json::getObjectOrArray(json, N_TARGETS);
  if (!optTarget) return false;
  auto& targetsArray = optTarget.get();
  if (!targetsArray.IsArray()) return false;
  msg::Target tTarget;
  for (auto& point : targetsArray.GetArray())
  {
    if (!tTarget.parseFromJson(point)) return false;
    m_targets.emplace_back(tTarget);
  }

  return true;
}

bool msg::ReadyRsp::parseFromProto(const std::string& msg)
{
  proto::ReadyRsp m;
  m.ParseFromString(msg);
  m_gameId = m.gameid();

  msg::Target tTarget;
  for (auto&& p : m.targets())
  {
    if (!tTarget.parseFromProto(p)) return false;
    m_targets.emplace_back(tTarget);
  }

  return true;
}

bool msg::ReadyRsp::parseFromXml(const std::string& msg)
{
  auto pDoc = new rapidxml::xml_document<>;
  char* cstr = new char[msg.size() + 1];
  strcpy(cstr, msg.c_str());
  pDoc->parse<0>(cstr);
  m_gameId = static_cast<int>(xml::getNumber(pDoc, N_GAME_ID));

  msg::Target tTarget;
  auto targets = xml::getObject(pDoc, N_TARGETS);

  for (; targets != nullptr; targets = targets->next_sibling(N_TARGETS.c_str()))
  {
    if (!tTarget.parseFromXml(targets)) return false;
    m_targets.emplace_back(tTarget);
  }

  delete[] cstr;
  delete pDoc;
  return true;
}

std::string msg::ReadyRsp::toJsonString() const
{
  rapidjson::Document doc(rapidjson::kObjectType);
  json::addIntToDoc(doc, N_GAME_ID, m_gameId);

  msg::Target tTarget;
  rapidjson::Value arr(rapidjson::kArrayType);

  for (auto&& p : m_targets)
  {
    rapidjson::Value obj(rapidjson::kObjectType);
    p.toJson(doc, obj);
    json::addObjectToArray(doc, arr, obj);
  }
  json::addArrayToDoc(doc, N_TARGETS, arr);
  return json::jsonToString(doc);
}

std::string msg::ReadyRsp::toProtoString() const
{
  proto::ReadyRsp msg;
  msg.set_gameid(m_gameId);

  for (auto&& t : m_targets)
  {
    auto target = msg.mutable_targets()->Add();
    t.toProto(target);
  }

  return msg.SerializeAsString();
}

std::string msg::ReadyRsp::toXMLString() const
{
  auto pDoc = new rapidxml::xml_document<>;
  xml::addDataToNode(pDoc, N_GAME_ID, static_cast<int>(m_gameId));

  for (auto&& p : m_targets)
  {
    auto targetsNode = xml::addDataToNode(pDoc, N_TARGETS);
    p.toXML(targetsNode);
  }

  auto toReturn = xml::xmlToString(pDoc);
  delete pDoc;
  return toReturn;
}
