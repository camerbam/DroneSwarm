#ifndef DOWN_MESSAGES_HPP
#define DOWN_MESSAGES_HPP

#include "DroneMessagesLib/OneParamMessage.hpp"

namespace messages
{
  class DownMessage : public OneParamMessage
  {
  public:
    DownMessage();
    DownMessage(double distance);

  private:
  };
} // namespace messages

#endif