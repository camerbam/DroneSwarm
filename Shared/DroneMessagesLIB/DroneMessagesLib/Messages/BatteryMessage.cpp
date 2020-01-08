#include "BatteryMessage.hpp"

messages::BatteryMessage::BatteryMessage() : NoParamMessage("battery")
{
}

std::string messages::BatteryMessage::toString() const
{
  return m_name + "?";
}

bool messages::BatteryMessage::verifyMessage(std::string& toParse,
                                             const std::string& messageName)
{
  auto nextWord = getNextWord(toParse);
  if (nextWord != (messageName + "?"))
    throw std::runtime_error(
      "Error: trying to parse message with incorrect Message type");
  return true;
}
