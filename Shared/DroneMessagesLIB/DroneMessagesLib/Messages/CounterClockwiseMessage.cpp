#include "CounterClockwiseMessage.hpp"

messages::CounterClockwiseMessage::CounterClockwiseMessage()
  : OneParamMessage("ccw", "angle", 1, 360)
{
}

messages::CounterClockwiseMessage::CounterClockwiseMessage(double distance)
  : OneParamMessage("ccw", "angle", 1, 360)
{
  if (validateDistance(distance)) m_firstArgument = distance;
}
