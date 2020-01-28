#ifndef XML_SERIALIZE_HPP
#define XML_SERIALIZE_HPP

#include <string>

#include <rapidxml/rapidxml.hpp>

namespace xml
{
  std::string xmlToString(rapidxml::xml_node<>* obj);

  rapidxml::xml_node<>* addDataToNode(rapidxml::xml_node<>* obj,
                                      const std::string& key);

  void addDataToNode(rapidxml::xml_node<>* obj,
                     const std::string& key,
                     const std::string& data);

  void addDataToNode(rapidxml::xml_node<>* obj,
                     const std::string& key,
                     const double& data);

  void addDataToNode(rapidxml::xml_node<>* obj,
                              const std::string& key,
                              const int& data); // TODO unit test

  void addDataToNode(rapidxml::xml_node<>* obj,
                     const std::string& key,
                     const bool& data);

} // namespace xml

#endif