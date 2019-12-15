#include "NoParamMessage.hpp"

#include "JSONLib/Deserialize.hpp"
#include "XMLLib/Deserialize.hpp"

messages::NoParamMessage::NoParamMessage(const std::string& name)
  : Message(name)
{
}

bool messages::NoParamMessage::fromStringImpl(std::string&)
{
  return true;
}

bool messages::NoParamMessage::readJsonImpl(rapidjson::Value&)
{
  return true;
}

bool messages::NoParamMessage::readXmlImpl(rapidxml::xml_node<>*)
{
  return true;
}
