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

#include <iomanip>
#include <iostream>

namespace
{
  const std::string CORRECT("Correct");
  const std::string NOT_RECEIVED("Not Received");
  const std::string OK("ok");

  class DroneSimulatorPretestMessages : public boost::static_visitor<bool>
  {
  public:
    DroneSimulatorPretestMessages(
      udp::UDPCommunicator& communicator,
      const boost::asio::ip::udp::endpoint& droneEndpoint)
      : m_messagesReceived(),
        m_communicator(communicator),
        m_droneEndpoint(droneEndpoint)
    {
      m_messagesReceived.emplace("BackMessage", false);
      m_messagesReceived.emplace("BatteryMessage", false);
      m_messagesReceived.emplace("ClockwiseMessage", false);
      // Had to have used it already
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

    bool handleMsg(const std::string& name, std::string msg)
    {
      m_messagesReceived[name] = true;
      m_communicator.sendMessage(msg, m_droneEndpoint);
      return false;
    }

    void printMsg()
    {
      std::cout << "************* RESULTS *************" << std::endl;
      for (auto&& m : m_messagesReceived)
      {
        std::cout << std::setw(50) << std::left << m.first << std::right;
        if (m.second)
          std::cout << CORRECT << std::endl;
        else
          std::cout << NOT_RECEIVED << std::endl;
      }
    }

    bool operator()(const messages::BackMessage&)
    {
      return handleMsg("BackMessage", OK);
    }

    bool operator()(const messages::BatteryMessage&)
    {
      return handleMsg("BatteryMessage", "95");
    }

    bool operator()(const messages::CommandMessage&)
    {
      return handleMsg("CommandMessage", OK);
    }

    bool operator()(const messages::ClockwiseMessage&)
    {
      return handleMsg("ClockwiseMessage", OK);
    }

    bool operator()(const messages::CounterClockwiseMessage&)
    {
      return handleMsg("CounterClockwiseMessage", OK);
    }

    bool operator()(const messages::DownMessage&)
    {
      return handleMsg("DownMessage", OK);
    }

    bool operator()(const messages::FlipMessage&)
    {
      return handleMsg("FlipMessage", OK);
    }

    bool operator()(const messages::ForwardMessage&)
    {
      return handleMsg("ForwardMessage", OK);
    }

    bool operator()(const messages::GoMessage&)
    {
      return handleMsg("GoMessage", OK);
    }

    bool operator()(const messages::LandMessage&)
    {
      handleMsg("LandMessage", OK);
      printMsg();
      return true;
    }

    bool operator()(const messages::LeftMessage&)
    {
      return handleMsg("LeftMessage", OK);
    }

    bool operator()(const messages::MDirectionMessage&)
    {
      return handleMsg("MDirectionMessage", OK);
    }

    bool operator()(const messages::MoffMessage&)
    {
      return handleMsg("MoffMessage", OK);
    }

    bool operator()(const messages::MonMessage&)
    {
      return handleMsg("MonMessage", OK);
    }

    bool operator()(const messages::RightMessage&)
    {
      return handleMsg("RightMessage", OK);
    }

    bool operator()(const messages::SpeedMessage&)
    {
      return handleMsg("SpeedMessage", OK);
    }

    bool operator()(const messages::TakeoffMessage&)
    {
      return handleMsg("TakeoffMessage", OK);
    }

    bool operator()(const messages::TimeMessage&)
    {
      return handleMsg("TimeMessage", OK);
    }

    bool operator()(const messages::UpMessage&)
    {
      return handleMsg("UpMessage", OK);
    }

  private:
    std::map<std::string, bool> m_messagesReceived;
    udp::UDPCommunicator& m_communicator;
    const boost::asio::ip::udp::endpoint m_droneEndpoint;
  };
}

drone::DroneSimulatorPretestState::DroneSimulatorPretestState(
  udp::UDPCommunicator& controlEndpoint,
  const boost::asio::ip::udp::endpoint& droneEndpoint)
  : DroneSimulatorState(controlEndpoint), m_droneEndpoint(droneEndpoint)
{
}

drone::DroneSimulatorPretestState::~DroneSimulatorPretestState()
{
}

std::shared_ptr<drone::DroneSimulatorState> drone::DroneSimulatorPretestState::
  handleResponse(const udp::Response& response)
{
  static DroneSimulatorPretestMessages pretest(m_sender, m_droneEndpoint);
  if (!response.didSucceed())
  {
    logger::logError("Pretest", "No messages recieved within timeout, exiting");
    return nullptr;
  }
  auto msg = response.getMessage();
  try
  {
    auto toExit = boost::apply_visitor(pretest, messages::getMessage(msg));
    if (toExit) return nullptr;
  }
  catch (std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
    return nullptr;
  }
  return shared_from_this();
}
