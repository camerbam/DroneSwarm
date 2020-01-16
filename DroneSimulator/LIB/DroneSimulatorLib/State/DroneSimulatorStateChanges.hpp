#ifndef DRONE_SIMULATOR_STATE_CHANGES_HPP
#define DRONE_SIMULATOR_STATE_CHANGES_HPP

#include <memory>
#include <string>

#include <boost/variant.hpp>

#include "DroneSimulatorStateImpl.hpp"

namespace messages
{
  class BackMessage;
  class BatteryMessage;
  class ClockwiseMessage;
  class CommandMessage;
  class CounterClockwiseMessage;
  class DownMessage;
  class FlipMessage;
  class ForwardMessage;
  class GoMessage;
  class LandMessage;
  class LeftMessage;
  class MDirectionMessage;
  class MoffMessage;
  class MonMessage;
  class RightMessage;
  class SpeedMessage;
  class TakeoffMessage;
  class TimeMessage;
  class UpMessage;
}
namespace drone
{
  class DroneSimulatorStateChanges
    : public boost::static_visitor<boost::optional<std::string>>
  {
  public:
    DroneSimulatorStateChanges(
      std::shared_ptr<DroneSimulatorStateImpl>& pState);

    boost::optional<std::string> operator()(
      const messages::BackMessage& message) const;

    boost::optional<std::string> operator()(
      const messages::BatteryMessage& message) const;

    boost::optional<std::string> operator()(
      const messages::ClockwiseMessage& message) const;

    boost::optional<std::string> operator()(
      const messages::CommandMessage& message) const;

    boost::optional<std::string> operator()(
      const messages::CounterClockwiseMessage& message) const;

    boost::optional<std::string> operator()(
      const messages::DownMessage& message) const;

    boost::optional<std::string> operator()(
      const messages::FlipMessage& message) const;

    boost::optional<std::string> operator()(
      const messages::ForwardMessage& message) const;

    boost::optional<std::string> operator()(
      const messages::GoMessage& message) const;

    boost::optional<std::string> operator()(
      const messages::LandMessage& message) const;

    boost::optional<std::string> operator()(
      const messages::LeftMessage& message) const;

    boost::optional<std::string> operator()(
      const messages::MDirectionMessage& message) const;

    boost::optional<std::string> operator()(
      const messages::MoffMessage& message) const;

    boost::optional<std::string> operator()(
      const messages::MonMessage& message) const;

    boost::optional<std::string> operator()(
      const messages::RightMessage& message) const;

    boost::optional<std::string> operator()(
      const messages::SpeedMessage& message) const;

    boost::optional<std::string> operator()(
      const messages::TakeoffMessage& message) const;

    boost::optional<std::string> operator()(
      const messages::TimeMessage& message) const;

    boost::optional<std::string> operator()(
      const messages::UpMessage& message) const;

  private:
    std::shared_ptr<DroneSimulatorStateImpl>& pState;
  };
} // namespace drone

#endif
