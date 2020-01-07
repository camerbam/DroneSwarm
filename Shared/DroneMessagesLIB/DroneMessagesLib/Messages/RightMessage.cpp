#include "RightMessage.hpp"

messages::RightMessage::RightMessage()
  : OneParamMessage("right", "distance", 20, 500)
{
}

messages::RightMessage::RightMessage(double distance)
  : OneParamMessage("right", "distance", 20, 500)
{
  if (validateDistance(distance))
  m_firstArgument = distance;
}
