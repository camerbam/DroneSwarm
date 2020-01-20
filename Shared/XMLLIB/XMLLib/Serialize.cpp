#include "Serialize.hpp"

#include <sstream>

#include <rapidxml/rapidxml_print.hpp>

namespace
{
  template <class T>
  void addData(T* xml, const std::string& key, const std::string& data)
  {
    auto pDoc = xml->document();
    if (!pDoc) return;

    char* pKey = pDoc->allocate_string(key.c_str(), key.length() + 1);
    char* pData = pDoc->allocate_string(data.c_str(), data.length() + 1);
    auto pNewNode = pDoc->allocate_node(rapidxml::node_element, pKey, pData);
    xml->append_node(pNewNode);
  }

  template <class T>
  rapidxml::xml_node<>* addNode(T* xml, const std::string& key)
  {
    auto pDoc = xml->document();
    if (!pDoc) return nullptr;

    char* pName = pDoc->allocate_string(key.c_str(), key.size() + 1);
    auto pNewNode = pDoc->allocate_node(rapidxml::node_element, pName);
    xml->append_node(pNewNode);

    return pNewNode;
  }
}

rapidxml::xml_node<>* xml::addDataToNode(rapidxml::xml_node<>* obj,
                                         const std::string& key)
{
  return ::addNode(obj, key);
}

std::string xml::xmlToString(rapidxml::xml_node<>* obj)
{
  std::stringstream ss;
  ss << *obj;
  return ss.str();
}

void xml::addDataToNode(rapidxml::xml_node<>* obj,
                        const std::string& key,
                        const std::string& data)
{
  ::addData(obj, key, data);
}

void xml::addDataToNode(rapidxml::xml_node<>* obj,
                        const std::string& key,
                        const double& data)
{
  ::addData(obj, key, std::to_string(data));
}

void xml::addDataToNode(rapidxml::xml_node<>* obj,
                        const std::string& key,
                        const int& data)
{
  ::addData(obj, key, std::to_string(data));
}

void xml::addDataToNode(rapidxml::xml_node<>* obj,
                        const std::string& key,
                        const bool& data)
{
  std::stringstream ss;
  ss << std::boolalpha << data;
  ::addData(obj, key, ss.str());
}
