#ifndef JSON_SERIALIZE_HPP
#define JSON_SERIALIZE_HPP

#include <rapidjson/document.h>

namespace json
{
  std::string jsonToString(rapidjson::Document& doc);
  std::string jsonToString(rapidjson::Value& doc);

  void addBoolToDoc(rapidjson::Document& doc,
                    const std::string& key,
                    const bool value);

  void addObjectToDoc(rapidjson::Document& doc,
                      const std::string& key,
                      rapidjson::Value& value);

  void addArrayToDoc(rapidjson::Document& doc,
                     const std::string& key,
                     rapidjson::Value& value);

  void addStringToDoc(rapidjson::Document& doc,
                      const std::string& key,
                      const std::string& value);

  void addNumberToDoc(rapidjson::Document& doc,
                      const std::string& key,
                      const double& value);

  // Add to Objects
  void addBoolToObject(rapidjson::Document& doc,
                       rapidjson::Value& obj,
                       const std::string& key,
                       const bool value);

  void addObjectToObject(rapidjson::Document& doc,
                         rapidjson::Value& obj,
                         const std::string& key,
                         rapidjson::Value& value);

  void addArrayToObject(rapidjson::Document& doc,
                        rapidjson::Value& obj,
                        const std::string& key,
                        rapidjson::Value& value);

  void addStringToObject(rapidjson::Document& doc,
                         rapidjson::Value& obj,
                         const std::string& member,
                         const std::string& value);

  void addNumberToObject(rapidjson::Document& doc,
                         rapidjson::Value& obj,
                         const std::string& member,
                         const double& value);

  // Add to Array
  void addBoolToArray(rapidjson::Document& doc,
                      rapidjson::Value& arr,
                      const bool value);

  void addObjectToArray(rapidjson::Document& doc,
                        rapidjson::Value& arr,
                        rapidjson::Value& value);

  void addArrayToArray(rapidjson::Document& doc,
                       rapidjson::Value& arr,
                       rapidjson::Value& value);

  void addStringToArray(rapidjson::Document& doc,
                        rapidjson::Value& arr,
                        const std::string& value);

  void addNumberToArray(rapidjson::Document& doc,
                        rapidjson::Value& arr,
                        const double& value);
} // namespace json

#endif