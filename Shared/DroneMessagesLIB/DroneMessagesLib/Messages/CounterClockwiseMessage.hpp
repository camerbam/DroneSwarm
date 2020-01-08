#ifndef COUNTER_CLOCKWISE_MESSAGES_HPP
#define COUNTER_CLOCKWISE_MESSAGES_HPP

#include "DroneMessagesLib/OneParamMessage.hpp"

namespace messages
{
  class CounterClockwiseMessage : public OneParamMessage
  {
  public:
    CounterClockwiseMessage();
    CounterClockwiseMessage(double distance);

  private:
  };
} // namespace messages

#endif