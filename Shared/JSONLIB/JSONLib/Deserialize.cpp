#include "Deserialize.hpp"

#include <functional>

namespace
{
  template <class T>
  bool getBool(T& obj, const std::string key)
  {
    auto itr = obj.FindMember(key.c_str());
    if (itr == obj.MemberEnd() || !itr->value.IsBool()) return false;
    return itr->value.GetBool();
  }

  template <class T>
  boost::optional<rapidjson::Value&> getObjectOrArray(T& obj, const std::string& key)
  {
    if(!obj.IsObject()) return boost::none;
    auto itr = obj.FindMember(key.c_str());
    if (itr == obj.MemberEnd() || (!itr->value.IsObject() && !itr->value.IsArray())){
      return boost::none;
    }
    return itr->value;
  }

  template <class T>
  std::string getString(T& obj, const std::string& key)
  {
    auto itr = obj.FindMember(key.c_str());
    if (itr == obj.MemberEnd() || !itr->value.IsString()) return std::string();
    return itr->value.GetString();
  }

  template <class T>
  double getNumber(T& obj, const std::string& key)
  {
    auto itr = obj.FindMember(key.c_str());
    if (itr == obj.MemberEnd() || !itr->value.IsNumber()) return 0;
    return itr->value.GetDouble();
  }

  template <class T, class RJ>
  std::vector<T> getVector(RJ& obj,
                           const std::string& key,
                           std::function<T(rapidjson::Value&)> f)
  {
    auto itr = obj.FindMember(key.c_str());
    if (itr == obj.MemberEnd() || !itr->value.IsArray()) return std::vector<T>();
    std::vector<T> toReturn;
    for (auto& v : itr->value.GetArray())
      toReturn.push_back(f(v));
    return toReturn;
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

boost::optional<rapidjson::Value&> json::getObjectOrArray(rapidjson::Document& doc,
                                         const std::string& key)
{
  return ::getObjectOrArray(doc, key);
}

boost::optional<rapidjson::Value&> json::getObjectOrArray(rapidjson::Value& obj,
                                         const std::string& key)
{
  return ::getObjectOrArray(obj, key);
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

std::vector<std::string> json::getVectorString(rapidjson::Document& doc,
                                               const std::string& key)
{
  return ::getVector<std::string>(
    doc, key, [](rapidjson::Value& val) -> std::string { return val.GetString(); });
}

std::vector<bool> json::getVectorBool(rapidjson::Document& doc,
                                      const std::string& key)
{
  return ::getVector<bool>(
    doc, key, [](rapidjson::Value& val) -> bool { return val.GetBool(); });
}

std::vector<double> json::getVectorDouble(rapidjson::Document& doc,
                                          const std::string& key)
{
  return ::getVector<double>(
    doc, key, [](rapidjson::Value& val) -> double { return val.GetDouble(); });
}
