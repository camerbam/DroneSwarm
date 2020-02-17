#include "HitTargetMsg.hpp"

#include <string>

#include <rapidjson/document.h>
#include <rapidxml/rapidxml.hpp>

#include "JSONLib/Deserialize.hpp"
#include "JSONLib/Serialize.hpp"
#include "XMLLib/Deserialize.hpp"
#include "XMLLib/Serialize.hpp"

#pragma warning(push)
#pragma warning(disable : 4267)
#include "ProtoLib/HitTargetMsg.pb.h"
#pragma warning(pop)

namespace
{
  const std::string N_GAME_ID("gameId");
  const std::string N_ID("id");
  const std::string N_TARGET("target");
} // namespace

msg::HitTargetMsg::HitTargetMsg(const int& gameId,
                                const int& id,
                                const TargetMsg& target)
  : m_gameId(gameId), m_id(id), m_target(target)
{
}

msg::HitTargetMsg::HitTargetMsg() : m_gameId(), m_id(0), m_target()
{
}

bool msg::HitTargetMsg::parseFromJson(const std::string& msg)
{
  rapidjson::Document json(rapidjson::kObjectType);
  json.Parse(msg.c_str());
  m_gameId = json::getInt(json, N_GAME_ID);
  m_id = json::getInt(json, N_ID);
  auto optTarget = json::getObjectOrArray(json, N_TARGET);
  if (!optTarget) return false;
  auto& target = optTarget.get();
  if (!m_target.parseFromJson(target)) return false;
  return true;
}

bool msg::HitTargetMsg::parseFromProto(const std::string& msg)
{
  proto::HitTargetMsg m;
  m.ParseFromString(msg);
  m_gameId = m.gameid();
  m_id = m.id();
  return m_target.parseFromProto(m.target());
}

bool msg::HitTargetMsg::parseFromXml(const std::string& msg)
{
  auto pDoc = new rapidxml::xml_document<>;
  char* cstr = new char[msg.size() + 1];
  strcpy(cstr, msg.c_str());
  pDoc->parse<0>(cstr);
  auto point = xml::getObject(pDoc, N_TARGET);

  m_gameId = static_cast<int>(xml::getNumber(pDoc, N_GAME_ID));
  m_id = static_cast<int>(xml::getNumber(pDoc, N_ID));
  if (!m_target.parseFromXml(point)) return false;

  delete[] cstr;
  delete pDoc;
  return true;
}

std::string msg::HitTargetMsg::toJsonString() const
{
  rapidjson::Document doc(rapidjson::kObjectType);
  rapidjson::Value obj(rapidjson::kObjectType);
  m_target.toJson(doc, obj);
  json::addIntToDoc(doc, N_GAME_ID, m_gameId);
  json::addIntToDoc(doc, N_ID, m_id);
  json::addObjectToDoc(doc, N_TARGET, obj);
  return json::jsonToString(doc);
}

std::string msg::HitTargetMsg::toProtoString() const
{
  proto::HitTargetMsg msg;
  m_target.toProto(msg.mutable_target());
  msg.set_id(m_id);
  msg.set_gameid(m_gameId);
  return msg.SerializeAsString();
}

std::string msg::HitTargetMsg::toXMLString() const
{
  auto pDoc = new rapidxml::xml_document<>;
  auto pointNode = xml::addDataToNode(pDoc, N_TARGET);
  xml::addDataToNode(pDoc, N_GAME_ID, m_gameId);
  xml::addDataToNode(pDoc, N_ID, m_id);
  m_target.toXML(pointNode);
  auto toReturn = xml::xmlToString(pDoc);
  delete pDoc;
  return toReturn;
}
