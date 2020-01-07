#include "UpMessage.hpp"

messages::UpMessage::UpMessage() : OneParamMessage("up", "distance", 20, 500)
{
}

messages::UpMessage::UpMessage(double distance)
  : OneParamMessage("up", "distance", 20, 500)
{
  if (validateDistance(distance)) m_firstArgument = distance;
}
