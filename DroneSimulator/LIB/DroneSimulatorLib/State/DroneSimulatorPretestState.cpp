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
  const std::string NEED_TO_TRY_AGAIN("Needs to be tried again");
  const std::string OK("ok");

  enum class Recieved
  {
    NOT_YET,
    RECEIVED,
    TRY_MSG_AGAIN
  };

  class DroneSimulatorPretestMessages : public boost::static_visitor<bool>
  {
  public:
    DroneSimulatorPretestMessages(
      udp::UDPCommunicator& communicator,
      const boost::asio::ip::udp::endpoint& droneEndpoint,
      bool reliable)
      : m_messagesReceived(),
        m_communicator(communicator),
        m_droneEndpoint(droneEndpoint),
        m_reliable(reliable)
    {
      m_messagesReceived.emplace("BackMessage", Recieved::NOT_YET);
      m_messagesReceived.emplace("BatteryMessage", Recieved::NOT_YET);
      m_messagesReceived.emplace("ClockwiseMessage", Recieved::NOT_YET);
      // Had to have used it already
      m_messagesReceived.emplace("CommandMessage", Recieved::RECEIVED);
      m_messagesReceived.emplace("CounterClockwiseMessage", Recieved::NOT_YET);
      m_messagesReceived.emplace("DownMessage", Recieved::NOT_YET);
      m_messagesReceived.emplace("FlipMessage", Recieved::NOT_YET);
      m_messagesReceived.emplace("ForwardMessage", Recieved::NOT_YET);
      m_messagesReceived.emplace("GoMessage", Recieved::NOT_YET);
      m_messagesReceived.emplace("LandMessage", Recieved::NOT_YET);
      m_messagesReceived.emplace("LeftMessage", Recieved::NOT_YET);
      m_messagesReceived.emplace("MDirectionMessage", Recieved::NOT_YET);
      m_messagesReceived.emplace("MoffMessage", Recieved::NOT_YET);
      m_messagesReceived.emplace("MonMessage", Recieved::NOT_YET);
      m_messagesReceived.emplace("RightMessage", Recieved::NOT_YET);
      m_messagesReceived.emplace("SpeedMessage", Recieved::NOT_YET);
      m_messagesReceived.emplace("TakeoffMessage", Recieved::NOT_YET);
      m_messagesReceived.emplace("TimeMessage", Recieved::NOT_YET);
      m_messagesReceived.emplace("UpMessage", Recieved::NOT_YET);
      if (!m_reliable) srand((unsigned int)time(NULL));
    }

    bool handleMsg(const std::string& name, std::string msg)
    {
      if (m_reliable)
      {
        m_messagesReceived[name] = Recieved::RECEIVED;
        m_communicator.sendMessage(msg, m_droneEndpoint);
      }
      else
      {
        auto chance = rand() % 100;
        if (chance > 90)
        {
          auto t = std::chrono::seconds(100 - chance);
          logger::logInfo(
            "Pretest",
            "Waiting " + std::to_string(t.count()) + " before responding");
          std::this_thread::sleep_for(t);
          m_communicator.sendMessage(msg, m_droneEndpoint);
          m_messagesReceived[name] = Recieved::RECEIVED;
        }
        else if (chance > 80)
        {
          logger::logInfo(
            "Pretest", "Not going to reply, need to send message again");
          m_messagesReceived[name] = Recieved::TRY_MSG_AGAIN;
          return false;
        }
        else if (chance > 70)
        {
          logger::logInfo(
            "Pretest", "Will send garbage back, need to send message again");
          m_messagesReceived[name] = Recieved::TRY_MSG_AGAIN;
          // RANDOMBYTES in Leet
          m_communicator.sendMessage("|24|\\||)0448`Y735", m_droneEndpoint);
          return false;
        }
        else
        {
          m_messagesReceived[name] = Recieved::RECEIVED;
          m_communicator.sendMessage(msg, m_droneEndpoint);
        }
      }
      return false;
    }

    void printMsg()
    {
      std::stringstream ss;
      ss << "************* RESULTS *************" << std::endl;
      for (auto&& m : m_messagesReceived)
      {
        ss << std::setw(40) << std::left << m.first << std::setw(25)
           << std::right;
        switch (m.second)
        {
        case Recieved::NOT_YET:
          ss << NOT_RECEIVED << std::endl;
          break;
        case Recieved::RECEIVED:
          ss << CORRECT << std::endl;
          break;
        case Recieved::TRY_MSG_AGAIN:
          ss << NEED_TO_TRY_AGAIN << std::endl;
          break;
        }
      }
      logger::logInfo("Pretest", ss.str());
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
    std::map<std::string, Recieved> m_messagesReceived;
    udp::UDPCommunicator& m_communicator;
    const boost::asio::ip::udp::endpoint m_droneEndpoint;
    bool m_reliable;
  };
}

drone::DroneSimulatorPretestState::DroneSimulatorPretestState(
  udp::UDPCommunicator& controlEndpoint,
  const boost::asio::ip::udp::endpoint& droneEndpoint,
  bool reliable)
  : DroneSimulatorState(controlEndpoint),
    m_droneEndpoint(droneEndpoint),
    m_reliable(reliable)
{
}

drone::DroneSimulatorPretestState::~DroneSimulatorPretestState()
{
}

std::shared_ptr<drone::DroneSimulatorState> drone::DroneSimulatorPretestState::
  handleResponse(const udp::Response& response)
{
  static DroneSimulatorPretestMessages pretest(
    m_sender, m_droneEndpoint, m_reliable);
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
