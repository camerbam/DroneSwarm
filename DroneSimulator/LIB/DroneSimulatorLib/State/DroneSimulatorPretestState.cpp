#include "DroneSimulatorPretestState.hpp"

#include "DroneMessagesLib/MessageFactory.hpp"
#include "DroneMessagesLib/Messages/BackMessage.hpp"
#include "DroneMessagesLib/Messages/BatteryMessage.hpp"
#include "DroneMessagesLib/Messages/BatteryMessage.hpp"
#include "DroneMessagesLib/Messages/ClockwiseMessage.hpp"
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
#include "DroneSimulatorPretestState.hpp"
#include "LoggerLib/Logger.hpp"

namespace
{
  class DroneSimulatorPretestMessages : public boost::static_visitor<bool>
  {
  public:
    DroneSimulatorPretestMessages() : m_messagesReceived()
    {
      m_messagesReceived.emplace("BackMessage", false);
      m_messagesReceived.emplace("BatteryMessage", false);
      m_messagesReceived.emplace("ClockwiseMessage", false);
      // Had to used it already
      m_messagesReceived.emplace("CommandMessage", true);
      m_messagesReceived.emplace("CounterClockwiseMessage", false);
      m_messagesReceived.emplace("DownMessage", false);
      m_messagesReceived.emplace("FlipMessage", false);
      m_messagesReceived.emplace("ForwardMessage", false);
      m_messagesReceived.emplace("GoMessage", false);
      m_messagesReceived.emplace("LandMessage", false);
      m_messagesReceived.emplace("LeftMessage", false);
      m_messagesReceived.emplace("MDirectionMessage", false);
      m_messagesReceived.emplace("MoffMessage", false);
      m_messagesReceived.emplace("MonMessage", false);
      m_messagesReceived.emplace("RightMessage", false);
      m_messagesReceived.emplace("SpeedMessage", false);
      m_messagesReceived.emplace("TakeoffMessage", false);
      m_messagesReceived.emplace("TimeMessage", false);
      m_messagesReceived.emplace("UpMessage", false);
    }

    bool DroneSimulatorPretestMessages::operator()(const messages::BackMessage&)
    {
      m_messagesReceived.at("BackMessage") = true;
    }

    bool DroneSimulatorPretestMessages::operator()(
      const messages::BatteryMessage&)
    {
      m_messagesReceived.at("BatteryMessage") = true;
    }

    bool DroneSimulatorPretestMessages::operator()(
      const messages::CommandMessage&)
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
      return returnAnswer(
        pState->enableDetection(static_cast<messages::DETECTION_DIRECTION>(
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

  private:
    std::map<std::string, bool> m_messagesReceived;
  };
}

drone::DroneSimulatorPretestState::DroneSimulatorPretestState(
  udp::UDPCommunicator& controlEndpoint,
  const boost::asio::ip::udp::endpoint& drone)
  : DroneSimulatorState(controlEndpoint)
{
}

drone::DroneSimulatorPretestState::~DroneSimulatorPretestState()
{
}

std::shared_ptr<drone::DroneSimulatorState> drone::DroneSimulatorPretestState::
  handleResponse(const udp::Response& response)
{
  static DroneSimulatorPretestMessages pretest;
  if (!response.didSucceed())
  {
    logger::logError("Pretest", "No messages recieved within timeout, exiting");
    return nullptr;
  }
  auto msg = response.getMessage();
  auto toExit = boost::apply_visitor(pretest, messages::getMessage(msg));
  return shared_from_this();
}
