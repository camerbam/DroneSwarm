#ifndef XML_DESERIALIZE_HPP
#define XML_DESERIALIZE_HPP

#include <vector>

#include <rapidxml/rapidxml.hpp>

namespace xml
{
  bool getBool(rapidxml::xml_document<>* doc, const std::string& key);

  bool getBool(rapidxml::xml_node<>* obj, const std::string& key);

  rapidxml::xml_node<>* getObject(rapidxml::xml_document<>* doc,
                                  const std::string& key);

  rapidxml::xml_node<>* getObject(rapidxml::xml_node<>* doc,
                                  const std::string& key);

  std::string getString(rapidxml::xml_document<>* doc, const std::string& key);

  std::string getString(rapidxml::xml_node<>* obj, const std::string& key);

  double getNumber(rapidxml::xml_document<>* doc, const std::string& key);

  double getNumber(rapidxml::xml_node<>* obj, const std::string& key);

  std::vector<std::string> getVectorString(rapidxml::xml_document<>* doc,
                                           const std::string& key);
  std::vector<std::string> getVectorString(rapidxml::xml_node<>* obj,
                                           const std::string& key);

  std::vector<bool> getVectorBool(rapidxml::xml_document<>* doc,
                                  const std::string& key);
  std::vector<bool> getVectorBool(rapidxml::xml_node<>* obj,
                                  const std::string& key);

  std::vector<double> getVectorDouble(rapidxml::xml_document<>* doc,
                                      const std::string& key);
  std::vector<double> getVectorDouble(rapidxml::xml_node<>* obj,
                                      const std::string& key);

} // namespace xml

#endif