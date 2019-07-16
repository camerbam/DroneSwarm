#ifndef JSON_DESERIALIZE_HPP
#define JSON_DESERIALIZE_HPP

#include <rapidjson/document.h>

namespace json
{
  bool getBool(rapidjson::Document& doc, const std::string& key);

  bool getBool(rapidjson::Value& obj, const std::string& key);

  rapidjson::Value& getObjectOrArray(rapidjson::Document& doc,
                                     const std::string& key);

  rapidjson::Value& getObjectOrArray(rapidjson::Value& doc,
                                     const std::string& key);

  std::string getString(rapidjson::Document& doc, const std::string& key);

  std::string getString(rapidjson::Value& obj, const std::string& key);

  double getNumber(rapidjson::Document& doc, const std::string& key);

  double getNumber(rapidjson::Value& obj, const std::string& key);

  //How to handle array
} // namespace json

#endif