#include "DownMessage.hpp"

messages::DownMessage::DownMessage()
  : OneParamMessage("down", "distance", 20, 500)
{
}
messages::DownMessage::DownMessage(double distance)
  : OneParamMessage("down", "distance", 20, 500)
{
  if (validateDistance(distance)) m_firstArgument = distance;
}
