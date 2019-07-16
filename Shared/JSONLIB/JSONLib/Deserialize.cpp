#include "Deserialize.hpp"

namespace
{
  template <class T>
  bool getBool(T& obj, const std::string key)
  {
    auto itr = obj.FindMember(key);
    if (itr == obj.MemberEnd() || !itr->value.IsBool()) return false;
    return itr->value.GetBool();
  }

  template <class T>
  rapidjson::Value& getObjectOrArray(T& obj, const std::string& key)
  {
    auto itr = obj.FindMember(key);
    if (itr == obj.MemberEnd() || !itr->value.IsObject())
      return rapidjson::Value(rapidjson::kObjectType);
    return itr->value.GetObject();
  }

  template <class T>
  std::string getString(T& obj, const std::string& key)
  {
    auto itr = obj.FindMember(key);
    if (itr == obj.MemberEnd() || !itr->value.IsString()) return std::string();
    return itr->value.GetString();
  }

  template <class T>
  double getNumber(T& obj, const std::string& key)
  {
    auto itr = obj.FindMember(key);
    if (itr == obj.MemberEnd() || !itr->value.IsNumber()) return 0;
    return itr->value.GetNumber();
  }
} // namespace

bool json::getBool(rapidjson::Document& doc, const std::string& key)
{
  return ::getBool(doc, key);
}

bool json::getBool(rapidjson::Value& obj, const std::string& key)
{
  return ::getBool(obj, key);
}

rapidjson::Value& json::getObjectOrArray(rapidjson::Document& doc,
                                         const std::string& key)
{
  return ::getObjectOrArray(doc, key);
}

rapidjson::Value& json::getObjectOrArray(rapidjson::Value& doc,
                                         const std::string& key)
{
  return ::getObjectOrArray(doc, key);
}

std::string json::getString(rapidjson::Document& doc, const std::string& key)
{
  return ::getString(doc, key);
}

std::string json::getString(rapidjson::Value& obj, const std::string& key)
{
  return ::getString(obj, key);
}

double json::getNumber(rapidjson::Document& doc, const std::string& key)
{
  return ::getNumber(doc, key);
}

double json::getNumber(rapidjson::Value& obj, const std::string& key)
{
  return ::getNumber(obj, key);
}
