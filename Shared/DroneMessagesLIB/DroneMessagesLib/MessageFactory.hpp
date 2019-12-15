#ifndef MESSAGE_FACTORY_HPP
#define MESSAGE_FACTORY_HPP

#include <string>

#include <rapidjson/document.h>
#include <rapidxml/rapidxml.hpp>

#include "Message_t.hpp"

namespace messages
{
  Message_t getMessage(rapidjson::Value& jsonToParse);
  Message_t getMessage(rapidxml::xml_node<>* xmlToParse);
  Message_t getMessage(std::string& stringToParse);

} // namespace messages

#endif