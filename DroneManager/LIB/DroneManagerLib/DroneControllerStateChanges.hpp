#ifndef DRONE_CONTROLLER_STATE_CHANGES_HPP
#define DRONE_CONTROLLER_STATE_CHANGES_HPP

#include <string>

#include <boost/variant.hpp>

#include "DroneControllerState.hpp"

namespace messages
{
  class GoMessage;
  class LandMessage;
  class MDirectionMessage;
  class MoffMessage;
  class MonMessage;
  class TakeoffMessage;
}

namespace drone
{
  class DroneControllerStateChanges : public boost::static_visitor<void>
  {
  public:
    DroneControllerStateChanges(std::shared_ptr<DroneControllerState>& pState);

    void operator()(const messages::GoMessage& message) const;

    void operator()(const messages::LandMessage& message) const;

    void operator()(const messages::MDirectionMessage& message) const;

    void operator()(const messages::MoffMessage& message) const;

    void operator()(const messages::MonMessage& message) const;

    void operator()(const messages::TakeoffMessage& message) const;

    template <class T>
    void operator()(const T&) const
    {
    }

  private:
    std::shared_ptr<DroneControllerState>& pState;
  };
} // namespace drone

#endif
