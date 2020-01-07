#ifndef RIGHT_MESSAGES_HPP
#define RIGHT_MESSAGES_HPP

#include "DroneMessagesLib/OneParamMessage.hpp"

namespace messages
{
  class RightMessage : public OneParamMessage
  {
  public:
    RightMessage();
    RightMessage(double distance);

  private:
  };
} // namespace messages

#endif
