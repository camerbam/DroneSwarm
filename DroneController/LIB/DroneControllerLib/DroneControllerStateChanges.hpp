#ifndef DRONE_CONTROLLER_STATE_CHANGES_HPP
#define DRONE_CONTROLLER_STATE_CHANGES_HPP

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
  class FlipMessage;
  class ForwardMessage;
  class GoMessage;
  class LandMessage;
  class LeftMessage;
  class RightMessage;
  class SpeedMessage;
  class TakeoffMessage;
  class TimeMessage;
  class UpMessage;
}

namespace drone
{
  class DroneControllerStateChanges : public boost::static_visitor<void>
  {
  public:
    DroneControllerStateChanges(std::shared_ptr<DroneControllerState>& pState);

    void operator()(const messages::BackMessage& message) const;

    void operator()(const messages::ClockwiseMessage& message) const;

    void operator()(const messages::CounterClockwiseMessage& message) const;

    void operator()(const messages::DownMessage& message) const;

    void operator()(const messages::FlipMessage& message) const;

    void operator()(const messages::ForwardMessage& message) const;

    void operator()(const messages::GoMessage& message) const;

    void operator()(const messages::LandMessage& message) const;

    void operator()(const messages::LeftMessage& message) const;

    void operator()(const messages::RightMessage& message) const;

    void operator()(const messages::TakeoffMessage& message) const;

    void operator()(const messages::UpMessage& message) const;

    template <class T>
    void operator()(const T&) const 
    {
    }

  private:
    std::shared_ptr<DroneControllerState>& pState;
  };
} // namespace drone

#endif
