
#include "DroneControllerCheckMsgToSend.hpp"

#include "DroneMessagesLib/Messages/BackMessage.hpp"
#include "DroneMessagesLib/Messages/BatteryMessage.hpp"
#include "DroneMessagesLib/Messages/ClockwiseMessage.hpp"
#include "DroneMessagesLib/Messages/CommandMessage.hpp"
#include "DroneMessagesLib/Messages/CounterClockwiseMessage.hpp"
#include "DroneMessagesLib/Messages/DownMessage.hpp"
#include "DroneMessagesLib/Messages/ForwardMessage.hpp"
#include "DroneMessagesLib/Messages/GoMessage.hpp"
#include "DroneMessagesLib/Messages/LandMessage.hpp"
#include "DroneMessagesLib/Messages/LeftMessage.hpp"
#include "DroneMessagesLib/Messages/MDirectionMessage.hpp"
#include "DroneMessagesLib/Messages/RightMessage.hpp"
#include "DroneMessagesLib/Messages/SpeedMessage.hpp"
#include "DroneMessagesLib/Messages/TakeoffMessage.hpp"
#include "DroneMessagesLib/Messages/TimeMessage.hpp"
#include "DroneMessagesLib/Messages/UpMessage.hpp"

namespace
{
  boost::optional<std::string> checkFlying(bool flying)
  {
    if (!flying) return "Drone is not flying";
    return boost::none;
  }
}

drone::DroneControllerCheckMsgToSend::DroneControllerCheckMsgToSend(
  std::shared_ptr<DroneControllerState>& pState)
  : pState(pState)
{
}

boost::optional<std::string> drone::DroneControllerCheckMsgToSend::operator()(
  const messages::BackMessage&) const
{
  return checkFlying(pState->isFlying());
}

boost::optional<std::string> drone::DroneControllerCheckMsgToSend::operator()(
  const messages::ClockwiseMessage&) const
{
  return checkFlying(pState->isFlying());
}

boost::optional<std::string> drone::DroneControllerCheckMsgToSend::operator()(
  const messages::CounterClockwiseMessage&) const
{
  return checkFlying(pState->isFlying());
}

boost::optional<std::string> drone::DroneControllerCheckMsgToSend::operator()(
  const messages::DownMessage&) const
{
  return checkFlying(pState->isFlying());
}

boost::optional<std::string> drone::DroneControllerCheckMsgToSend::operator()(
  const messages::ForwardMessage&) const
{
  return checkFlying(pState->isFlying());
}

boost::optional<std::string> drone::DroneControllerCheckMsgToSend::operator()(
  const messages::GoMessage&) const
{
  auto error = checkFlying(pState->isFlying());
  if (error) return error;
  error = checkFlying(pState->isFlying());
  if (error) return error;
  return checkFlying(pState->isFlying());
}

boost::optional<std::string> drone::DroneControllerCheckMsgToSend::operator()(
  const messages::LandMessage&) const
{
  if (!pState->isFlying()) return "Drone is already landed";
  return boost::none;
}

boost::optional<std::string> drone::DroneControllerCheckMsgToSend::operator()(
  const messages::LeftMessage&) const
{
  return checkFlying(pState->isFlying());
}

boost::optional<std::string> drone::DroneControllerCheckMsgToSend::operator()(
  const messages::MDirectionMessage&) const
{
  if (pState->getDirection() == messages::DETECTION_DIRECTION::NONE)
    return "Cannot set detection when it is not enabled";
  return boost::none;
}

boost::optional<std::string> drone::DroneControllerCheckMsgToSend::operator()(
  const messages::RightMessage&) const
{
  return checkFlying(pState->isFlying());
}

boost::optional<std::string> drone::DroneControllerCheckMsgToSend::operator()(
  const messages::TakeoffMessage&) const
{
  if (pState->isFlying()) return "Drone is already flying";
  if (pState->getBattery() < 5) return "Drone is out of battery";
  return boost::none;
}

boost::optional<std::string> drone::DroneControllerCheckMsgToSend::operator()(
  const messages::UpMessage&) const
{
  return checkFlying(pState->isFlying());
}
