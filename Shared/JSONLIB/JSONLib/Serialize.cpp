#include "Serialize.hpp"

#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

namespace
{
  template<class T>
  addBool(rapidjson::Document& doc, T& obj, const std::string& key, const bool value)
  {
    rapidjson::Value rapidValue(value);
    rapidjson::Value index(
      key.c_str(), static_cast<int>(key.size()), doc.GetAllocator());
    obj.AddMember(index, rapidValue, doc.GetAllocator());
  }

  template<class T>
  addObjectOrArray(rapidjson::Document& doc, T& obj, const std::string& key, rapidjson::Value& value)
  {
    rapidjson::Value rapidValue(rapidjson::kObjectType);
    rapidValue = value;
    rapidjson::Value index(
      member.c_str(), static_cast<int>(member.size()), doc.GetAllocator());
    obj.AddMember(index, rapidValue, doc.GetAllocator());
  }

  template<class T>
  addString(rapidjson::Document& doc, T& obj, const std::string& key, const std::string& value)
  {
    rapidjson::Value rapidValue(
      value.c_str(), static_cast<int>(value.size()), doc.GetAllocator());
    rapidjson::Value index(
      key.c_str(), static_cast<int>(key.size()), doc.GetAllocator());
    obj.AddMember(index, rapidValue, doc.GetAllocator());
  }

  template<class T>
  addNumber(rapidjson::Document& doc, T& obj, const std::string& key, const double& value)
  {
    rapidjson::Value rapidValue;
    rapidValue.SetDouble(value);
    rapidjson::Value index(
      key.c_str(), static_cast<int>(key.size()), doc.GetAllocator());
    obj.AddMember(index, rapidValue, doc.GetAllocator());
  }
}

std::string json::jsonToString(const rapidjson::Document& doc)
{
  rapidjson::StringBuffer buffer;
  buffer.Clear();
  rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
  doc.Accept(writer);
  return std::string(buffer.GetString());
}

void json::addBoolToDoc(rapidjson::Document& doc,
                        const std::string& key,
                        bool value)
{
  ::addBool(doc, doc, key, value);
}

void json::addObjectToDoc(rapidjson::Document& doc,
                          const std::string& key,
                          rapidjson::Value& value)
{
  ::addObjectOrArray(doc, doc, key, value);
}

void json::addArrayToDoc(rapidjson::Document& doc,
                         const std::string& key,
                         rapidjson::Value& value)
{
  ::addObjectOrArray(doc, doc, key, value);
}

void json::addStringToDoc(rapidjson::Document& doc,
                          const std::string& key,
                          const std::string& value)
{
  ::addString(doc, doc, key, value);
}

void json::addNumberToDoc(rapidjson::Document& doc,
                          const std::string& key,
                          const double& value)
{
  ::addNumber(doc, doc, key, value);
}

void json::addBoolToObject(rapidjson::Document& doc,
                           rapidjson::Value& obj,
                           const std::string& key,
                           const bool value)
{
  ::addBool(doc, obj, key, value);
}

void json::addObjectToObject(rapidjson::Document& doc,
                             rapidjson::Value& obj,
                             const std::string& key,
                             rapidjson::Value& value)
{
  ::addObjectOrArray(doc, obj, key, value);
}

void json::addArrayToObject(rapidjson::Document& doc,
                            rapidjson::Value& obj,
                            const std::string& key,
                            rapidjson::Value& value)
{
  ::addObjectOrArray(doc, obj, key, value);
}

void json::addStringToObject(rapidjson::Document& doc,
                             rapidjson::Value& obj,
                             const std::string& key,
                             const std::string& value)
{
  ::addString(doc, obj, key, value);
}

void json::addNumberToObject(rapidjson::Document& doc,
                             rapidjson::Value& obj,
                             const std::string& key,
                             const double& value)
{
  ::addNumber(doc, obj, key, value);
}

void json::addBoolToArray(rapidjson::Document& doc,
                          rapidjson::Value& arr,
                          const bool value)
{
  rapidjson::Value rapidValue(value);
  arr.PushBack(rapidValue, doc.GetAllocator());
}

void json::addObjectToArray(rapidjson::Document& doc,
                            rapidjson::Value& arr,
                            rapidjson::Value& value)
{
  rapidjson::Value rapidValue(rapidjson::kObjectType);
  rapidValue = value;
  arr.PushBack(rapidValue, doc.GetAllocator());
}

void json::addArrayToArray(rapidjson::Document& doc,
                           rapidjson::Value& arr,
                           rapidjson::Value& value)
{
  rapidjson::Value rapidValue(rapidjson::kArrayType);
  rapidValue = value;
  arr.PushBack(rapidValue, doc.GetAllocator());
}

void json::addStringToArray(rapidjson::Document& doc,
                            rapidjson::Value& arr,
                            const std::string& value)
{
  rapidjson::Value rapidValue(
    value.c_str(), static_cast<int>(value.size()), doc.GetAllocator());
  arr.PushBack(rapidValue, doc.GetAllocator());
}

void json::addNumberToArray(rapidjson::Document& doc,
                            rapidjson::Value& arr,
                            const double& value)
{
  rapidjson::Value rapidValue(value);
  arr.PushBack(rapidValue, doc.GetAllocator());
}
