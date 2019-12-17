#include "Message.hpp"

namespace
{
  const size_t N_PRECISION(2);
}

messages::Message::Message(const std::string& name) : m_name(name) {}

bool messages::Message::fromString(std::string& toParse)
{
  if (!verifyMessage(toParse, m_name)) return false;
  return fromStringImpl(toParse);
}

std::string messages::Message::toString() const
{
  return m_name;
}

std::string messages::Message::getNextWord(std::string& toParse)
{
  size_t space = toParse.find(" ");
  auto toReturn = toParse.substr(0, space);
  if (space == std::string::npos)
    toParse.clear();
  else
    toParse.erase(0, space + 1);
  return toReturn;
}

std::string messages::Message::removeUnimportantZeros(std::string toTrim) const
{
  size_t period = toTrim.find(".");
  if (period == std::string::npos) return toTrim;
  size_t i(0);
  for (i = period + N_PRECISION; i > period && toTrim[i] == '0'; i--)
    ;
  return toTrim.substr(0, i);
}

bool messages::Message::verifyMessage(std::string& toParse,
                                      const std::string& messageName)
{
  auto nextWord = getNextWord(toParse);
  if (nextWord != messageName)
    throw std::runtime_error(
      "Error: trying to parse message with incorrect Message type");
  return true;
}
