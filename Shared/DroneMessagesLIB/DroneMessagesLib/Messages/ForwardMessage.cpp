#include "ForwardMessage.hpp"

messages::ForwardMessage::ForwardMessage()
  : OneParamMessage("forward", "distance", 20, 500)
{
}

messages::ForwardMessage::ForwardMessage(double distance)
  : OneParamMessage("forward", "distance", 20, 500)
{
  if (validateDistance(distance)) m_firstArgument = distance;
}
