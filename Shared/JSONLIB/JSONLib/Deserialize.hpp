#ifndef JSON_DESERIALIZE_HPP
#define JSON_DESERIALIZE_HPP

#include <vector>

#include <boost/optional.hpp>

#include <rapidjson/document.h>

namespace json
{
  bool getBool(rapidjson::Document& doc, const std::string& key);

  bool getBool(rapidjson::Value& obj, const std::string& key);

  boost::optional<rapidjson::Value&> getObjectOrArray(rapidjson::Document& doc,
                                     const std::string& key);

  boost::optional<rapidjson::Value&> getObjectOrArray(rapidjson::Value& doc,
                                     const std::string& key);

  std::string getString(rapidjson::Document& doc, const std::string& key);

  std::string getString(rapidjson::Value& obj, const std::string& key);

  double getNumber(rapidjson::Document& doc, const std::string& key);

  double getNumber(rapidjson::Value& obj, const std::string& key);

  std::vector<std::string> getVectorString(rapidjson::Document& doc, const std::string& key);

  std::vector<bool> getVectorBool(rapidjson::Document& doc, const std::string& key);

  std::vector<double> getVectorDouble(rapidjson::Document& doc, const std::string& key);
} // namespace json

#endif