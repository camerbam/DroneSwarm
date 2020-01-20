#ifndef DRONE_CONTROLLER_CHECK_MSG_TO_SEND_HPP
#define DRONE_CONTROLLER_CHECK_MSG_TO_SEND_HPP

#include <string>

#include <boost/variant.hpp>

#include "DroneControllerState.hpp"

namespace messages
{
  class BackMessage;
  class BatteryMessage;
  class ClockwiseMessage;
  class CommandMessage;
  class CounterClockwiseMessage;
  class DownMessage;
  class ForwardMessage;
  class GoMessage;
  class LandMessage;
  class LeftMessage;
  class MDirectionMessage;
  class RightMessage;
  class SpeedMessage;
  class TakeoffMessage;
  class TimeMessage;
  class UpMessage;
}

namespace drone
{
  class DroneControllerCheckMsgToSend : public boost::static_visitor<boost::optional<std::string>>
  {
  public:
    DroneControllerCheckMsgToSend(std::shared_ptr<DroneControllerState>& pState);

    boost::optional<std::string> operator()(const messages::BackMessage& message) const;

    boost::optional<std::string> operator()(const messages::ClockwiseMessage& message) const;

    boost::optional<std::string> operator()(const messages::CounterClockwiseMessage& message) const;

    boost::optional<std::string> operator()(const messages::DownMessage& message) const;

    boost::optional<std::string> operator()(const messages::ForwardMessage& message) const;

    boost::optional<std::string> operator()(const messages::GoMessage& message) const;

    boost::optional<std::string> operator()(const messages::LandMessage& message) const;

    boost::optional<std::string> operator()(const messages::LeftMessage& message) const;

    boost::optional<std::string> operator()(const messages::MDirectionMessage& message) const;

    boost::optional<std::string> operator()(const messages::RightMessage& message) const;

    boost::optional<std::string> operator()(const messages::TakeoffMessage& message) const;

    boost::optional<std::string> operator()(const messages::UpMessage& message) const;

    template <class T>
    boost::optional<std::string> operator()(const T&) const
    {
      return boost::none;
    }

  private:
    const std::shared_ptr<DroneControllerState>& pState;
  };
} // namespace drone

#endif
