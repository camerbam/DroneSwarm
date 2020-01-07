#include "Deserialize.hpp"

#include <string>

namespace
{
  template <class T>
  bool getBool(const T* node, const std::string& key)
  {
    auto pNodeFound = node->first_node(key.c_str(), key.size());
    if (!pNodeFound) return false;
    if (pNodeFound->value() == std::string("true")) return true;
    return false;
  }

  template <class T>
  rapidxml::xml_node<>* getObject(const T* node, const std::string& key)
  {
    return node->first_node(key.c_str(), key.size());
  }

  template <class T>
  std::string getString(const T* node, const std::string& key)
  {
    auto pNodeFound = node->first_node(key.c_str(), key.size());
    if (!pNodeFound) return std::string();
    return pNodeFound->value();
  }

  template <class T>
  double getNumber(const T* node, const std::string& key)
  {
    auto pNodeFound = node->first_node(key.c_str(), key.size());
    if (!pNodeFound) return 0;
    try
    {
      return std::stod(pNodeFound->value());
    }
    catch (...)
    {
    }
    return 0;
  }

  template <class T>
  std::vector<std::string> getVectorString(T* doc, const std::string& key)
  {
    auto pNodeFound = doc->first_node(key.c_str(), key.size());
    std::vector<std::string> toReturn;
    while (pNodeFound)
    {
      toReturn.emplace_back(pNodeFound->value());
      pNodeFound = pNodeFound->next_sibling(key.c_str(), key.size());
    }
    return toReturn;
  }

  template <class T>
  std::vector<bool> getVectorBool(T* doc, const std::string& key)
  {
    auto pNodeFound = doc->first_node(key.c_str(), key.size());
    std::vector<bool> toReturn;
    while (pNodeFound)
    {
      if (pNodeFound->value() == std::string("true"))
        toReturn.push_back(true);
      else
        toReturn.push_back(false);
      pNodeFound = pNodeFound->next_sibling(key.c_str(), key.size());
    }
    return toReturn;
  }

  template <class T>
  std::vector<double> getVectorDouble(T* doc, const std::string& key)
  {
    auto pNodeFound = doc->first_node(key.c_str(), key.size());
    std::vector<double> toReturn;
    while (pNodeFound)
    {
      try
      {
        toReturn.push_back(std::stod(pNodeFound->value()));
      }
      catch (...)
      {
      }
      pNodeFound = pNodeFound->next_sibling(key.c_str(), key.size());
    }
    return toReturn;
  }
}

bool xml::getBool(rapidxml::xml_document<>* doc, const std::string& key)
{
  return ::getBool(doc, key);
}

bool xml::getBool(rapidxml::xml_node<>* obj, const std::string& key)
{
  return ::getBool(obj, key);
}

rapidxml::xml_node<>* xml::getObject(rapidxml::xml_document<>* doc,
                                     const std::string& key)
{
  return ::getObject(doc, key);
}

rapidxml::xml_node<>* xml::getObject(rapidxml::xml_node<>* doc,
                                     const std::string& key)
{
  return ::getObject(doc, key);
}

std::string xml::getString(rapidxml::xml_document<>* doc,
                           const std::string& key)
{
  return ::getString(doc, key);
}

std::string xml::getString(rapidxml::xml_node<>* obj, const std::string& key)
{
  return ::getString(obj, key);
}

double xml::getNumber(rapidxml::xml_document<>* doc, const std::string& key)
{
  return ::getNumber(doc, key);
}

double xml::getNumber(rapidxml::xml_node<>* obj, const std::string& key)
{
  return ::getNumber(obj, key);
}

std::vector<std::string> xml::getVectorString(rapidxml::xml_document<>* doc,
                                              const std::string& key)
{
  return ::getVectorString(doc, key);
}

std::vector<std::string> xml::getVectorString(rapidxml::xml_node<>* obj,
                                              const std::string& key)
{
  return ::getVectorString(obj, key);
}

std::vector<bool> xml::getVectorBool(rapidxml::xml_document<>* doc,
                                     const std::string& key)
{
  return ::getVectorBool(doc, key);
}

std::vector<bool> xml::getVectorBool(rapidxml::xml_node<>* obj,
                                     const std::string& key)
{
  return ::getVectorBool(obj, key);
}

std::vector<double> xml::getVectorDouble(rapidxml::xml_document<>* doc,
                                         const std::string& key)
{
  return ::getVectorDouble(doc, key);
}

std::vector<double> xml::getVectorDouble(rapidxml::xml_node<>* obj,
                                         const std::string& key)
{
  return ::getVectorDouble(obj, key);
}
