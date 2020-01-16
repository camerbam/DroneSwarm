#include "BackMessage.hpp"

messages::BackMessage::BackMessage()
  : OneParamMessage("back", 20, 500)
{
}

messages::BackMessage::BackMessage(const double& distance)
  : OneParamMessage("back", 20, 500)
{
  if (validateDistance(distance))
  m_firstArgument = distance;
}
