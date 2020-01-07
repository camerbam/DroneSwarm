#include "SpeedMessage.hpp"

messages::SpeedMessage::SpeedMessage() : NoParamMessage("speed")
{
}

std::string messages::SpeedMessage::toString() const
{
  return m_name + "?";
}

bool messages::SpeedMessage::verifyMessage(std::string& toParse,
  const std::string& messageName)
{
  auto nextWord = getNextWord(toParse);
  if (nextWord != (messageName + "?"))
    throw std::runtime_error(
      "Error: trying to parse message with incorrect Message type");
  return true;
}
