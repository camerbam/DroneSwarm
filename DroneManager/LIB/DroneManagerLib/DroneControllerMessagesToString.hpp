#ifndef DRONE_CONTROLLER_MESSAGES_TO_STRING_HPP
#define DRONE_CONTROLLER_MESSAGES_TO_STRING_HPP

#include <string>

#include <boost/variant/static_visitor.hpp>

#include "DroneControllerState.hpp"

#include "DroneMessagesLib/Messages/FlipMessage.hpp"

namespace drone
{
  class DroneControllerMessagesToString
    : public boost::static_visitor<std::string>
  {
  public:
    DroneControllerMessagesToString(
      std::shared_ptr<DroneControllerState>& pState);

    std::string operator()(messages::FlipMessage message) const;

    template <class T>
    std::string operator()(T message) const
    {
      return message.toString();
    }

  private:
    std::shared_ptr<DroneControllerState>& pState;
  };
} // namespace drone

#endif
