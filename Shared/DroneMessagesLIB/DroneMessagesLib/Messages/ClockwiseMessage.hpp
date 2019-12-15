#ifndef CLOCKWISE_MESSAGES_HPP
#define CLOCKWISE_MESSAGES_HPP

#include "DroneMessagesLib/OneParamMessage.hpp"

namespace messages
{
  class ClockwiseMessage : public OneParamMessage
  {
  public:
    ClockwiseMessage();
    ClockwiseMessage(double distance);

  private:
  };
} // namespace messages

#endif