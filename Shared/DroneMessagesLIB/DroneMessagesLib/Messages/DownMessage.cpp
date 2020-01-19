#include "DownMessage.hpp"

messages::DownMessage::DownMessage()
  : OneParamMessage("down", 20, 500)
{
}
messages::DownMessage::DownMessage(double distance)
  : OneParamMessage("down", 20, 500)
{
  if (validateDistance(distance)) m_firstArgument = distance;
}
