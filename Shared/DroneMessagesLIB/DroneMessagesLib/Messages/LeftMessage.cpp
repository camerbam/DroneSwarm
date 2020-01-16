#include "LeftMessage.hpp"

messages::LeftMessage::LeftMessage()
  : OneParamMessage("left", 20, 500)
{
}

messages::LeftMessage::LeftMessage(double distance)
  : OneParamMessage("left", 20, 500)
{
  if (validateDistance(distance)) m_firstArgument = distance;
}
