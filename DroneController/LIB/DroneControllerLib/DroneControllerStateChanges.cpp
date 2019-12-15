
#include "DroneControllerStateChanges.hpp"

#include "DroneMessagesLib/Messages/BackMessage.hpp"
#include "DroneMessagesLib/Messages/BatteryMessage.hpp"
#include "DroneMessagesLib/Messages/ClockwiseMessage.hpp"
#include "DroneMessagesLib/Messages/CommandMessage.hpp"
#include "DroneMessagesLib/Messages/CounterClockwiseMessage.hpp"
#include "DroneMessagesLib/Messages/DownMessage.hpp"
#include "DroneMessagesLib/Messages/FlipMessage.hpp"
#include "DroneMessagesLib/Messages/ForwardMessage.hpp"
#include "DroneMessagesLib/Messages/GoMessage.hpp"
#include "DroneMessagesLib/Messages/LandMessage.hpp"
#include "DroneMessagesLib/Messages/LeftMessage.hpp"
#include "DroneMessagesLib/Messages/RightMessage.hpp"
#include "DroneMessagesLib/Messages/SpeedMessage.hpp"
#include "DroneMessagesLib/Messages/TakeoffMessage.hpp"
#include "DroneMessagesLib/Messages/TimeMessage.hpp"
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
  const messages::ClockwiseMessage& message) const
{
  pState->changeAngle(-message.getArgument());
}

void drone::DroneControllerStateChanges::operator()(
  const messages::CounterClockwiseMessage& message) const
{
  pState->changeAngle(message.getArgument());
}

void drone::DroneControllerStateChanges::operator()(
  const messages::DownMessage& message) const
{
  pState->changeZ(-message.getArgument());
}

void drone::DroneControllerStateChanges::operator()(
  const messages::FlipMessage& message) const
{
  switch (message.getDirection())
  {
  case messages::Direction::FORWARD:
    pState->changeX(20);
    break;
  case messages::Direction::RIGHT:
    pState->changeY(20);
    break;
  case messages::Direction::BACK:
    pState->changeX(-20);
    break;
  case messages::Direction::LEFT:
    pState->changeY(-20);
    break;
  }
}

void drone::DroneControllerStateChanges::operator()(
  const messages::ForwardMessage& message) const
{
  pState->changeX(message.getArgument());
}

void drone::DroneControllerStateChanges::operator()(
  const messages::GoMessage& message) const
{
  pState->changeX(message.getXDistance());
  pState->changeY(message.getYDistance());
  pState->changeZ(message.getZDistance());
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
