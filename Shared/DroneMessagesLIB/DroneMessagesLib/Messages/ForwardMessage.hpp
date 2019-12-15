#ifndef FORWARD_MESSAGES_HPP
#define FORWARD_MESSAGES_HPP

#include "DroneMessagesLib/OneParamMessage.hpp"

namespace messages
{
  class ForwardMessage : public OneParamMessage
  {
  public:
    ForwardMessage();
    ForwardMessage(double distance);

  private:
  };
} // namespace messages

#endif