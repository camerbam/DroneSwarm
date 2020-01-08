#include "TimeMessage.hpp"

messages::TimeMessage::TimeMessage() : NoParamMessage("time")
{
}

std::string messages::TimeMessage::toString() const
{
  return m_name + "?";
}

bool messages::TimeMessage::verifyMessage(std::string& toParse,
  const std::string& messageName)
{
  auto nextWord = getNextWord(toParse);
  if (nextWord != (messageName + "?"))
    throw std::runtime_error(
      "Error: trying to parse message with incorrect Message type");
  return true;
}
