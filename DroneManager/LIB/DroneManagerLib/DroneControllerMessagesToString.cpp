
#include "DroneControllerMessagesToString.hpp"

#include "DroneMessagesLib/Messages/BackMessage.hpp"
#include "DroneMessagesLib/Messages/ForwardMessage.hpp"
#include "DroneMessagesLib/Messages/LeftMessage.hpp"
#include "DroneMessagesLib/Messages/RightMessage.hpp"

drone::DroneControllerMessagesToString::DroneControllerMessagesToString(
  std::shared_ptr<DroneControllerState>& pState)
  : pState(pState)
{
}

std::string drone::DroneControllerMessagesToString::operator()(
  messages::FlipMessage message) const
{
  if (pState->getBattery() < 50)
  {
    switch (message.getDirection())
    {
    case messages::Direction::FORWARD:
      return messages::ForwardMessage(20).toString();
    case messages::Direction::BACK:
      return messages::BackMessage(20).toString();
    case messages::Direction::RIGHT:
      return messages::RightMessage(20).toString();
    case messages::Direction::LEFT:
      return messages::LeftMessage(20).toString();
    }
  }
  return message.toString();
}
