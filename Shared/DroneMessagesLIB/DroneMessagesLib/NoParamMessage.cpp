#include "NoParamMessage.hpp"

messages::NoParamMessage::NoParamMessage(const std::string& name)
  : Message(name)
{
}

bool messages::NoParamMessage::fromStringImpl(std::string&)
{
  return true;
}
