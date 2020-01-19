#include "MDirectionMessage.hpp"

#include <iostream>

messages::MDirectionMessage::MDirectionMessage()
  : OneParamMessage("mdirection", 0, 2)
{
}

messages::MDirectionMessage::MDirectionMessage(double direction)
  : OneParamMessage("mdirection", 0, 2)
{
  if (validateDistance(direction)) m_firstArgument = direction;
}
