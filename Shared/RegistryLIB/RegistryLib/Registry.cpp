#include "Registry.hpp"

#include <fstream>
#include <iostream>

#include <boost/asio/post.hpp>
#include <boost/filesystem/operations.hpp>

#include <rapidjson/istreamwrapper.h>

#include "JSONLib/Deserialize.hpp"
#include "JSONLib/Serialize.hpp"
#include "UtilsLib/Utils.hpp"

namespace
{
  double validateDValue(double count, double def)
  {
    if (utils::compareTwoDoubles(count, 0)) return def;
    return count;
  }

  int validateValue(int count, int def)
  {
    if (!count) return def;
    return count;
  }

  std::vector<Target> validateTargets(rapidjson::Document& doc)
  {
    auto targetsArray = doc.FindMember("Targets");
    if (targetsArray == doc.MemberEnd()) return {};
    if (!targetsArray->value.IsArray()) return {};

    std::vector<Target> toReturn;
    for (auto& target : targetsArray->value.GetArray())
    {
      auto x = target.FindMember("x");
      if (x == target.MemberEnd() || !x->value.IsNumber()) continue;
      auto y = target.FindMember("y");
      if (y == target.MemberEnd() || !y->value.IsNumber()) continue;
      auto id = target.FindMember("id");
      if (id == target.MemberEnd() || !id->value.IsInt()) continue;
      std::set<int> depends;
      auto dependents = target.FindMember("Dependents");
      if (dependents != target.MemberEnd() && dependents->value.IsArray())
      {
        for (auto& d : dependents->value.GetArray())
        {
          if (d.IsInt()) depends.emplace(d.GetInt());
        }
      }
      auto falseAfter = target.FindMember("FalseAfter");
      int f = 0;
      if (falseAfter != target.MemberEnd() && falseAfter->value.IsInt())
        f = falseAfter->value.GetInt();
      toReturn.emplace_back(
        x->value.GetInt(), y->value.GetInt(), id->value.GetInt(), depends, f);
    }
    return toReturn;
  }
}

GlobalRegistry::GlobalRegistry(const std::string& config)
  : m_pThreadPool(), m_speedRatio(1), m_decaySpeed(2)
{
  parseConfig(config);
}

GlobalRegistry::GlobalRegistry()
  : m_pThreadPool(std::make_shared<boost::asio::thread_pool>(1)),
    m_speedRatio(1),
    m_decaySpeed(2)
{
}

void GlobalRegistry::setRegistry()
{
  m_pInstance = std::shared_ptr<GlobalRegistry>(new GlobalRegistry());
}

void GlobalRegistry::setRegistry(const boost::filesystem::path& p)
{
  if (!boost::filesystem::exists(p))
    throw std::runtime_error("Cannot find file: " + p.generic_string());
  if (boost::filesystem::is_directory(p))
    throw std::runtime_error("file is a directory: " + p.generic_string());
  std::ifstream t(p.string());
  std::string str(
    (std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
  setRegistry(str);
}

void GlobalRegistry::setRegistry(const std::string& config)
{
  m_pInstance = std::shared_ptr<GlobalRegistry>(new GlobalRegistry(config));
}

void GlobalRegistry::setRegistry(double speed,
                                 double decaySpeed,
                                 const std::vector<Target>& targets,
                                 bool skipLog,
                                 bool encrypted)
{
  rapidjson::Document doc(rapidjson::kObjectType);
  json::addNumberToDoc(doc, "ThreadCount", 1);
  json::addNumberToDoc(doc, "Speed", speed);
  json::addNumberToDoc(doc, "BatteryDecaySpeed", decaySpeed);
  if (!targets.empty())
  {
    rapidjson::Document targetsJson(rapidjson::kArrayType);
    for (auto&& target : targets)
    {
      rapidjson::Document targetJson(rapidjson::kObjectType);
      json::addIntToObject(doc, targetJson, "x", target.getX());
      json::addIntToObject(doc, targetJson, "y", target.getY());
      json::addIntToObject(doc, targetJson, "id", target.getId());

      if (!target.getDependents().empty())
      {
        rapidjson::Document dependents(rapidjson::kArrayType);

        for (auto&& id : target.getDependents())
        {
          json::addIntToArray(doc, dependents, id);
        }
        json::addArrayToObject(doc, targetJson, "Dependents", dependents);
      }
      if (target.getFalseAfter() != 0)
      {
        json::addIntToObject(
          doc, targetJson, "FalseAfter", target.getFalseAfter());
      }

      json::addObjectToArray(doc, targetsJson, targetJson);
    }
    json::addArrayToDoc(doc, "Targets", targetsJson);
  }
  json::addBoolToDoc(doc, "SkipLog", skipLog);
  json::addBoolToDoc(doc, "Encrypted", encrypted);
  setRegistry(json::jsonToString(doc));
}

GlobalRegistry& GlobalRegistry::getRegistry()
{
  if (!m_pInstance) throw std::runtime_error("Invalid Registry");
  return *m_pInstance;
}

GlobalRegistry::~GlobalRegistry()
{
  m_pInstance->m_pThreadPool->join();
}

void GlobalRegistry::postToThreadPool(std::function<void()> fn)
{
  boost::asio::post(*m_pInstance->m_pThreadPool, fn);
}

double GlobalRegistry::getSpeedRatio()
{
  return m_pInstance->m_speedRatio;
}

double GlobalRegistry::getBatteryDecaySpeed()
{
  return m_pInstance->m_decaySpeed;
}

const std::vector<Target>& GlobalRegistry::getTargets()
{
  return m_pInstance->m_targets;
}

bool GlobalRegistry::getSkipLog()
{
  return m_pInstance->m_skipLog;
}

bool GlobalRegistry::getPrintLog()
{
  return m_pInstance->m_printLog;
}

size_t GlobalRegistry::getPretest()
{
  return m_pInstance->m_pretest;
}

bool GlobalRegistry::isEncypted()
{
  return m_pInstance->m_encypted;
}

bool GlobalRegistry::parseConfig(const std::string& config)
{
  rapidjson::Document doc;
  doc.Parse(config.c_str());

  if (doc.IsNull())
    throw std::runtime_error("Config does not point to valid json");

  if (!m_pThreadPool)
    m_pThreadPool = std::make_shared<boost::asio::thread_pool>(
      validateValue(static_cast<int>(json::getNumber(doc, "ThreadCount")), 1));
  m_speedRatio = validateDValue(json::getNumber(doc, "Speed"), 2);
  m_decaySpeed = validateDValue(json::getNumber(doc, "BatteryDecaySpeed"), 1);
  m_targets = validateTargets(doc);
  m_skipLog = json::getBool(doc, "SkipLog");
  m_pretest = json::getInt(doc, "Pretest");
  m_encypted = json::getBool(doc, "Encrypted");
  m_printLog = json::getBool(doc, "PrintToConsole");

  return true;
}

std::shared_ptr<GlobalRegistry> GlobalRegistry::m_pInstance;
