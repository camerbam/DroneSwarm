#include "HitTargetRsp.hpp"

#include <rapidjson/document.h>
#include <rapidxml/rapidxml.hpp>

#include "JSONLib/Deserialize.hpp"
#include "JSONLib/Serialize.hpp"
#include "XMLLib/Deserialize.hpp"
#include "XMLLib/Serialize.hpp"

#pragma warning(push)
#pragma warning(disable : 4267)
#include "ProtoLib/HitTargetRsp.pb.h"
#pragma warning(pop)

namespace
{
  const std::string N_SUCESS("success");
  const std::string N_COMPLETE("complete");
  const std::string N_NEW_TARGETS("newTargets");
  const std::string N_BAD_TARGETS("badTargets");
}

msg::HitTargetRsp::HitTargetRsp()
  : m_success(), m_complete(), m_newTargets(), m_badTargets()
{
}

msg::HitTargetRsp::HitTargetRsp(bool success,
                                bool complete,
                                const std::vector<TargetMsg>& newTargets,
                                const std::vector<TargetMsg>& badTargets)
  : m_success(success),
    m_complete(complete),
    m_newTargets(newTargets),
    m_badTargets(badTargets)
{
}

bool msg::HitTargetRsp::parseFromJson(const std::string& msg)
{
  rapidjson::Document json(rapidjson::kObjectType);
  json.Parse(msg.c_str());
  m_success = json::getBool(json, N_SUCESS);
  m_complete = json::getBool(json, N_COMPLETE);

  {
    auto optTarget = json::getObjectOrArray(json, N_NEW_TARGETS);
    if (!optTarget) return false;
    auto& targetsArray = optTarget.get();
    if (!targetsArray.IsArray()) return false;
    msg::TargetMsg tTarget;
    for (auto& point : targetsArray.GetArray())
    {
      if (!tTarget.parseFromJson(point)) return false;
      m_newTargets.emplace_back(tTarget);
    }
  }
  {
    auto optTarget = json::getObjectOrArray(json, N_BAD_TARGETS);
    if (!optTarget) return false;
    auto& targetsArray = optTarget.get();
    if (!targetsArray.IsArray()) return false;
    msg::TargetMsg tTarget;
    for (auto& point : targetsArray.GetArray())
    {
      if (!tTarget.parseFromJson(point)) return false;
      m_badTargets.emplace_back(tTarget);
    }
  }

  return true;
}

bool msg::HitTargetRsp::parseFromProto(const std::string& msg)
{
  proto::HitTargetRsp m;
  m.ParseFromString(msg);
  m_success = m.success();
  m_complete = m.complete();

  msg::TargetMsg tTarget;
  {
    for (auto&& p : m.newtargets())
    {
      if (!tTarget.parseFromProto(p)) return false;
      m_newTargets.emplace_back(tTarget);
    }
  }

  {
    for (auto&& p : m.badtargets())
    {
      if (!tTarget.parseFromProto(p)) return false;
      m_badTargets.emplace_back(tTarget);
    }
  }
  return true;
}

bool msg::HitTargetRsp::parseFromXml(const std::string& msg)
{
  auto pDoc = new rapidxml::xml_document<>;
  char* cstr = new char[msg.size() + 1];
  strcpy(cstr, msg.c_str());
  pDoc->parse<0>(cstr);
  m_success = xml::getBool(pDoc, N_SUCESS);
  m_complete = xml::getBool(pDoc, N_COMPLETE);

  msg::TargetMsg tTarget;
  {
    auto targets = xml::getObject(pDoc, N_NEW_TARGETS);

    for (; targets != nullptr;
         targets = targets->next_sibling(N_NEW_TARGETS.c_str()))
    {
      if (!tTarget.parseFromXml(targets)) return false;
      m_newTargets.emplace_back(tTarget);
    }
  }
  {
    auto targets = xml::getObject(pDoc, N_BAD_TARGETS);

    for (; targets != nullptr;
         targets = targets->next_sibling(N_BAD_TARGETS.c_str()))
    {
      if (!tTarget.parseFromXml(targets)) return false;
      m_badTargets.emplace_back(tTarget);
    }
  }

  delete[] cstr;
  delete pDoc;
  return true;
}

std::string msg::HitTargetRsp::toJsonString() const
{
  rapidjson::Document doc(rapidjson::kObjectType);
  json::addBoolToDoc(doc, N_SUCESS, m_success);
  json::addBoolToDoc(doc, N_COMPLETE, m_complete);

  msg::TargetMsg tTarget;
  {
    rapidjson::Value arr(rapidjson::kArrayType);

    for (auto&& p : m_newTargets)
    {
      rapidjson::Value obj(rapidjson::kObjectType);
      p.toJson(doc, obj);
      json::addObjectToArray(doc, arr, obj);
    }
    json::addArrayToDoc(doc, N_NEW_TARGETS, arr);
  }
  {
    rapidjson::Value arr(rapidjson::kArrayType);
    for (auto&& p : m_badTargets)
    {
      rapidjson::Value obj(rapidjson::kObjectType);
      p.toJson(doc, obj);
      json::addObjectToArray(doc, arr, obj);
    }
    json::addArrayToDoc(doc, N_BAD_TARGETS, arr);
  }
  return json::jsonToString(doc);
}

std::string msg::HitTargetRsp::toProtoString() const
{
  proto::HitTargetRsp msg;
  msg.set_success(m_success);
  msg.set_complete(m_complete);

  for (auto&& p : m_newTargets)
  {
    auto point = msg.mutable_newtargets()->Add();
    p.toProto(point);
  }
  
  for (auto&& p : m_badTargets)
  {
    auto point = msg.mutable_badtargets()->Add();
    p.toProto(point);
  }
  return msg.SerializeAsString();
}

std::string msg::HitTargetRsp::toXMLString() const
{
  auto pDoc = new rapidxml::xml_document<>;
  xml::addDataToNode(pDoc, N_SUCESS, m_success);
  xml::addDataToNode(pDoc, N_COMPLETE, m_complete);

  for (auto&& p : m_newTargets)
  {
    auto targetsNode = xml::addDataToNode(pDoc, N_NEW_TARGETS);
    p.toXML(targetsNode);
  }

  for (auto&& p : m_badTargets)
  {
    auto targetsNode = xml::addDataToNode(pDoc, N_BAD_TARGETS);
    p.toXML(targetsNode);
  }
  auto toReturn = xml::xmlToString(pDoc);
  delete pDoc;
  return toReturn;
}
