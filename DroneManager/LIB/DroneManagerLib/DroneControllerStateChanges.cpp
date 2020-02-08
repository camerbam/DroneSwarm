
#include "DroneControllerStateChanges.hpp"

#include "DroneMessagesLib/Messages/BackMessage.hpp"
#include "DroneMessagesLib/Messages/DownMessage.hpp"
#include "DroneMessagesLib/Messages/ForwardMessage.hpp"
#include "DroneMessagesLib/Messages/GoMessage.hpp"
#include "DroneMessagesLib/Messages/LandMessage.hpp"
#include "DroneMessagesLib/Messages/LeftMessage.hpp"
#include "DroneMessagesLib/Messages/MDirectionMessage.hpp"
#include "DroneMessagesLib/Messages/MoffMessage.hpp"
#include "DroneMessagesLib/Messages/MonMessage.hpp"
#include "DroneMessagesLib/Messages/RightMessage.hpp"
#include "DroneMessagesLib/Messages/TakeoffMessage.hpp"
#include "DroneMessagesLib/Messages/UpMessage.hpp"

drone::DroneControllerStateChanges::DroneControllerStateChanges(
  std::shared_ptr<DroneControllerState>& pState)
  : pState(pState)
{
}

void drone::DroneControllerStateChanges::operator()(
  const messages::BackMessage& message) const
{
  pState->changeX(-message.getArgument());
}

void drone::DroneControllerStateChanges::operator()(
  const messages::DownMessage& message) const
{
  pState->changeZ(-message.getArgument());
}

void drone::DroneControllerStateChanges::operator()(
  const messages::ForwardMessage& message) const
{
  pState->changeX(message.getArgument());
}

void drone::DroneControllerStateChanges::operator()(
  const messages::GoMessage& message) const
{
  pState->changeSpeed(message.getSpeed());
}

void drone::DroneControllerStateChanges::operator()(
  const messages::LandMessage&) const
{
  pState->land();
}

void drone::DroneControllerStateChanges::operator()(
  const messages::LeftMessage& message) const
{
  pState->changeY(-message.getArgument());
}

void drone::DroneControllerStateChanges::operator()(
  const messages::MDirectionMessage& message) const
{
  pState->setDetection(static_cast<messages::DETECTION_DIRECTION>(
    static_cast<int>(message.getArgument())));
}

void drone::DroneControllerStateChanges::operator()(
  const messages::MoffMessage&) const
{
  pState->setDetection(messages::DETECTION_DIRECTION::NONE);
}

void drone::DroneControllerStateChanges::operator()(
  const messages::MonMessage&) const
{
  pState->setDetection(messages::DETECTION_DIRECTION::BOTH);
}

void drone::DroneControllerStateChanges::operator()(
  const messages::RightMessage& message) const
{
  pState->changeY(message.getArgument());
}

void drone::DroneControllerStateChanges::operator()(
  const messages::TakeoffMessage&) const
{
  pState->takeoff();
}

void drone::DroneControllerStateChanges::operator()(
  const messages::UpMessage& message) const
{
  pState->changeZ(message.getArgument());
}
