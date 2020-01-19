#include "UpMessage.hpp"

messages::UpMessage::UpMessage() : OneParamMessage("up", 20, 500)
{
}

messages::UpMessage::UpMessage(double distance)
  : OneParamMessage("up", 20, 500)
{
  if (validateDistance(distance)) m_firstArgument = distance;
}
