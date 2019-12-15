#include "ClockwiseMessage.hpp"

messages::ClockwiseMessage::ClockwiseMessage()
  : OneParamMessage("cw", "angle", 1, 360)
{
}

messages::ClockwiseMessage::ClockwiseMessage(double distance)
  : OneParamMessage("cw", "angle", 1, 360)
{
  if (validateDistance(distance)) m_firstArgument = distance;
}
