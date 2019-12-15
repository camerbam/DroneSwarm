#include "FlipMessage.hpp"

#include <exception>
#include <map>

#include "JSONLib/Deserialize.hpp"
#include "XMLLib/Deserialize.hpp"

namespace
{
  std::map<std::string, messages::Direction> stringToDirection = {
    {"f", messages::Direction::FORWARD},
    {"r", messages::Direction::RIGHT},
    {"b", messages::Direction::BACK},
    {"l", messages::Direction::LEFT}};

  std::map<messages::Direction, std::string> directionToString = {
    {messages::Direction::FORWARD, "f"},
    {messages::Direction::RIGHT, "r"},
    {messages::Direction::BACK, "b"},
    {messages::Direction::LEFT, "l"},
  };
}

messages::FlipMessage::FlipMessage() : Message("flip")
{
}

messages::FlipMessage::FlipMessage(std::string toParse) : Message("flip")
{
  if(!fromStringImpl(toParse))
   throw std::runtime_error("Could not instantiate flip message with: " + toParse);
}

bool messages::FlipMessage::fromStringImpl(std::string& toParse)
{
  auto direction = stringToDirection.find(getNextWord(toParse));
  if (direction == stringToDirection.end()) return false;
  m_direction = direction->second;
  return true;
}

std::string messages::FlipMessage::toString() const
{
  return m_name + " " + directionToString[m_direction];
}

messages::Direction messages::FlipMessage::getDirection() const
{
  return m_direction;
}

bool messages::FlipMessage::readJsonImpl(rapidjson::Value& jsonToParse)
{
  auto direction = json::getString(jsonToParse, "direction");
  auto directionItr = stringToDirection.find(direction);
  if (directionItr == stringToDirection.end())
    throw std::runtime_error("Unknow direction");
  m_direction = directionItr->second;
  return true;
}

bool messages::FlipMessage::readXmlImpl(rapidxml::xml_node<>* xmlToParse)
{
  auto direction = xml::getString(xmlToParse, "direction");
  auto directionItr = stringToDirection.find(direction);
  if (directionItr == stringToDirection.end())
    throw std::runtime_error("Unknow direction");
  m_direction = directionItr->second;
  return true;
}
