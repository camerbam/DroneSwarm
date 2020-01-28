
#include "DroneSimulatorStateChanges.hpp"

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
#include "DroneMessagesLib/Messages/MDirectionMessage.hpp"
#include "DroneMessagesLib/Messages/MoffMessage.hpp"
#include "DroneMessagesLib/Messages/MonMessage.hpp"
#include "DroneMessagesLib/Messages/RightMessage.hpp"
#include "DroneMessagesLib/Messages/SpeedMessage.hpp"
#include "DroneMessagesLib/Messages/TakeoffMessage.hpp"
#include "DroneMessagesLib/Messages/TimeMessage.hpp"
#include "DroneMessagesLib/Messages/UpMessage.hpp"

namespace
{
  const std::string OK("ok");

  boost::optional<std::string> returnError(
    const std::string& error,
    boost::optional<std::string> defaultValue = boost::none)
  {
    if (error.empty()) return defaultValue;
    return error;
  }

  boost::optional<std::string> returnAnswer(
    const boost::optional<std::string>& error)
  {
    if (error) return error;
    return OK;
  }
}

drone::DroneSimulatorStateChanges::DroneSimulatorStateChanges(
  std::shared_ptr<DroneSimulatorStateImpl>& pState)
  : pState(pState)
{
}

boost::optional<std::string> drone::DroneSimulatorStateChanges::operator()(
  const messages::BackMessage& message) const
{
  return returnError(pState->changeTargetY(-message.getArgument()));
}

boost::optional<std::string> drone::DroneSimulatorStateChanges::operator()(
  const messages::BatteryMessage&) const
{
  return std::to_string(pState->getBattery());
}

boost::optional<std::string> drone::DroneSimulatorStateChanges::operator()(
  const messages::CommandMessage&) const
{
  if (!pState) pState.reset(new DroneSimulatorStateImpl());
  return OK;
}

boost::optional<std::string> drone::DroneSimulatorStateChanges::operator()(
  const messages::ClockwiseMessage& message) const
{
  return returnError(pState->changeTargetAngle(-message.getArgument()));
}

boost::optional<std::string> drone::DroneSimulatorStateChanges::operator()(
  const messages::CounterClockwiseMessage& message) const
{
  return returnError(pState->changeTargetAngle(message.getArgument()));
}

boost::optional<std::string> drone::DroneSimulatorStateChanges::operator()(
  const messages::DownMessage& message) const
{
  return returnError(pState->changeTargetZ(-message.getArgument()));
}

boost::optional<std::string> drone::DroneSimulatorStateChanges::operator()(
  const messages::FlipMessage& message) const
{
  std::string error;
  switch (message.getDirection())
  {
  case messages::Direction::FORWARD:
    error = pState->changeTargetY(20);
    break;
  case messages::Direction::RIGHT:
    error = pState->changeTargetX(20);
    break;
  case messages::Direction::BACK:
    error = pState->changeTargetY(-20);
    break;
  case messages::Direction::LEFT:
    error = pState->changeTargetX(-20);
    break;
  }
  return returnError(error);
}

boost::optional<std::string> drone::DroneSimulatorStateChanges::operator()(
  const messages::ForwardMessage& message) const
{
  return returnError(pState->changeTargetY(message.getArgument()));
}

boost::optional<std::string> drone::DroneSimulatorStateChanges::operator()(
  const messages::GoMessage& message) const
{
  std::string error(pState->changeTargetXYZ(message.getXDistance(),
                                            message.getYDistance(),
                                            message.getZDistance(),
                                            message.getSpeed()));
  return returnError(error);
}

boost::optional<std::string> drone::DroneSimulatorStateChanges::operator()(
  const messages::LandMessage&) const
{
  return returnError(pState->land());
}

boost::optional<std::string> drone::DroneSimulatorStateChanges::operator()(
  const messages::LeftMessage& message) const
{
  return returnError(pState->changeTargetX(-message.getArgument()));
}

boost::optional<std::string> drone::DroneSimulatorStateChanges::operator()(
  const messages::MDirectionMessage& message) const
{
  return returnAnswer(pState->enableDetection(static_cast<messages::DETECTION_DIRECTION>(
    static_cast<int>(message.getArgument()))));
}

boost::optional<std::string> drone::DroneSimulatorStateChanges::operator()(
  const messages::MoffMessage&) const
{
  pState->disableDetection();
  return OK;
}

boost::optional<std::string> drone::DroneSimulatorStateChanges::operator()(
  const messages::MonMessage&) const
{
  return returnAnswer(pState->enableDetection());
}

boost::optional<std::string> drone::DroneSimulatorStateChanges::operator()(
  const messages::RightMessage& message) const
{
  return returnError(pState->changeTargetX(message.getArgument()));
}

boost::optional<std::string> drone::DroneSimulatorStateChanges::operator()(
  const messages::SpeedMessage&) const
{
  return std::to_string(pState->getSpeed());
}

boost::optional<std::string> drone::DroneSimulatorStateChanges::operator()(
  const messages::TakeoffMessage&) const
{
  return returnError(pState->takeoff());
}

boost::optional<std::string> drone::DroneSimulatorStateChanges::operator()(
  const messages::TimeMessage&) const
{
  return std::to_string(pState->getTime());
}

boost::optional<std::string> drone::DroneSimulatorStateChanges::operator()(
  const messages::UpMessage& message) const
{
  return returnError(pState->changeTargetZ(message.getArgument()));
}
